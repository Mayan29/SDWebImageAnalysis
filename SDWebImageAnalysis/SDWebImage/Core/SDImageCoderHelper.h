/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <ImageIO/ImageIO.h>
#import "SDWebImageCompat.h"
#import "SDImageFrame.h"

// Provide some common helper methods for building the image decoder/encoder.
// 提供一些构建图像解码器/编码器的常用辅助方法。
@interface SDImageCoderHelper : NSObject

// Return an animated image with frames array.
// For UIKit, this will apply the patch and then create animated UIImage. The patch is because that `+[UIImage animatedImageWithImages:duration:]` just use the average of duration for each image. So it will not work if different frame has different duration. Therefore we repeat the specify frame for specify times to let it work.
// For AppKit, NSImage does not support animates other than GIF. This will try to encode the frames to GIF format and then create an animated NSImage for rendering. Attention the animated image may loss some detail if the input frames contain full alpha channel because GIF only supports 1 bit alpha channel. (For 1 pixel, either transparent or not)
// 返回带帧数组的动画图像。
// 对于 UIKit，这将应用补丁，然后创建动画 UIImage。这个补丁是因为 `+[UIImage animatedImageWithImages:duration:]` 仅使用每个图像的平均持续时间。如果不同的帧有不同的持续时间，它将不起作用。因此，我们重复指定时间的指定帧以使其工作。
// 对于 AppKit，NSImage 不支持 GIF 以外的动画。这将尝试将帧编码为 GIF 格式，然后创建用于渲染的动画 NSImage。注意，如果输入帧包含完整的 alpha 通道，动画图像可能会丢失一些细节，因为 GIF 只支持 1 位 alpha 通道。（1 个像素，透明或不透明）
+ (UIImage * _Nullable)animatedImageWithFrames:(NSArray<SDImageFrame *> * _Nullable)frames;

// Return frames array from an animated image.
// For UIKit, this will unapply the patch for the description above and then create frames array. This will also work for normal animated UIImage.
// For AppKit, NSImage does not support animates other than GIF. This will try to decode the GIF imageRep and then create frames array.
// 根据动画图像返回帧数组。
// 对于 UIKit，这将取消应用上述描述的补丁，然后创建 frames 数组。这也适用于普通的动画 UIImage。
// 对于 AppKit，NSImage 不支持 GIF 以外的动画。这将尝试解码 GIF imageRep，然后创建帧数组。
+ (NSArray<SDImageFrame *> * _Nullable)framesFromAnimatedImage:(UIImage * _Nullable)animatedImage NS_SWIFT_NAME(frames(from:));

// Return the shared device-dependent RGB color space. This follows The Get Rule.
// On iOS, it's created with deviceRGB (if available, use sRGB).
// On macOS, it's from the screen colorspace (if failed, use deviceRGB)
// Because it's shared, you should not retain or release this object.
// 返回与共享设备相关的 RGB 颜色空间。遵循 Get 规则。
// 在 iOS 上，它是用 deviceRGB 创建的（如果可以得到，请使用 sRGB）
// 在 MacOS 上，它来自屏幕颜色空间（如果失败，请使用 deviceRGB）
// 因为它是共享的，所以不应保留或释放此对象。
+ (CGColorSpaceRef _Nonnull)colorSpaceGetDeviceRGB CF_RETURNS_NOT_RETAINED;

// Check whether CGImage contains alpha channel.
// 检查 CGImage 是否包含 alpha 通道。
+ (BOOL)CGImageContainsAlpha:(_Nonnull CGImageRef)cgImage;

// Create a decoded CGImage by the provided CGImage. This follows The Create Rule and you are response to call release after usage.
// It will detect whether image contains alpha channel, then create a new bitmap context with the same size of image, and draw it. This can ensure that the image do not need extra decoding after been set to the imageView.
// @note This actually call `CGImageCreateDecoded:orientation:` with the Up orientation.
// 通过提供的 CGImage 创建解码的 CGImage。这遵循创建规则，您将在使用后响应调用 release。
// 检测图像是否包含 alpha 通道，然后创建与图像大小相同的新位图上下文，并绘制。这可以确保图像在设置为 imageView 之后不需要额外的解码。
// 注意：这实际上是用 Up 方向调用 `CGImageCreateDecoded:orientation:`
+ (CGImageRef _Nullable)CGImageCreateDecoded:(_Nonnull CGImageRef)cgImage CF_RETURNS_RETAINED;

// Create a decoded CGImage by the provided CGImage and orientation. This follows The Create Rule and you are response to call release after usage.
// It will detect whether image contains alpha channel, then create a new bitmap context with the same size of image, and draw it. This can ensure that the image do not need extra decoding after been set to the imageView.
// orientation: The EXIF image orientation.
// 通过提供的 CGImage 和方向创建解码的 CGImage。这遵循创建规则，您将在使用后响应调用 release。
// 检测图像是否包含 alpha 通道，然后创建与图像大小相同的新位图上下文，并绘制。这可以确保图像在设置为 imageView 之后不需要额外的解码。
// orientation: EXIF 图像方向。
+ (CGImageRef _Nullable)CGImageCreateDecoded:(_Nonnull CGImageRef)cgImage orientation:(CGImagePropertyOrientation)orientation CF_RETURNS_RETAINED;

// Return the decoded image by the provided image. This one unlike `CGImageCreateDecoded:`, will not decode the image which contains alpha channel or animated image
// 根据提供的图像返回解码图像。这个与 `CGImageCreateDecoded:` 不同，它不会解码包含 alpha 通道或动画图像的图像
+ (UIImage * _Nullable)decodedImageWithImage:(UIImage * _Nullable)image;

/**
 Return the decoded and probably scaled down image by the provided image. If the image is large than the limit size, will try to scale down. Or just works as `decodedImageWithImage:`

 @param image The image to be decoded and scaled down
 @param bytes The limit bytes size. Provide 0 to use the build-in limit.
 @return The decoded and probably scaled down image
 */
+ (UIImage * _Nullable)decodedAndScaledDownImageWithImage:(UIImage * _Nullable)image limitBytes:(NSUInteger)bytes;

#if SD_UIKIT || SD_WATCH
/**
 Convert an EXIF image orientation to an iOS one.

 @param exifOrientation EXIF orientation
 @return iOS orientation
 */
+ (UIImageOrientation)imageOrientationFromEXIFOrientation:(CGImagePropertyOrientation)exifOrientation NS_SWIFT_NAME(imageOrientation(from:));

/**
 Convert an iOS orientation to an EXIF image orientation.

 @param imageOrientation iOS orientation
 @return EXIF orientation
 */
+ (CGImagePropertyOrientation)exifOrientationFromImageOrientation:(UIImageOrientation)imageOrientation;
#endif

@end
