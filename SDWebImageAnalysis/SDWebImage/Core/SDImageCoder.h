/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "NSData+ImageContentType.h"

typedef NSString * SDImageCoderOption NS_STRING_ENUM;
typedef NSDictionary<SDImageCoderOption, id> SDImageCoderOptions;
typedef NSMutableDictionary<SDImageCoderOption, id> SDImageCoderMutableOptions;

#pragma mark - Coder Options

// These options are for image decoding
// 这些选项用于图像解码

// A Boolean value indicating whether to decode the first frame only for animated image during decoding. (NSNumber). If not provide, decode animated image if need.
// @note works for `SDImageCoder`.
// 一个布尔值，表示在解码期间是否仅对动画图像解码第一帧。如果不提供，请在需要时解码动画图像。
// 注意：适用于 `SDImageCoder`。
FOUNDATION_EXPORT SDImageCoderOption _Nonnull const SDImageCoderDecodeFirstFrameOnly;

// A CGFloat value which is greater than or equal to 1.0. This value specify the image scale factor for decoding. If not provide, use 1.0. (NSNumber)
// @note works for `SDImageCoder`, `SDProgressiveImageCoder`, `SDAnimatedImageCoder`.
// CGFloat 值大于或等于 1.0。该值指定用于解码的图像比例。如果不提供，默认使用 1.0。
// 注意：适用于 `SDImageCoder`、`SDProgressiveImageCoder`、`SDAnimatedImageCoder`。
FOUNDATION_EXPORT SDImageCoderOption _Nonnull const SDImageCoderDecodeScaleFactor;

// These options are for image encoding
// 这些选项用于图像编码

// A Boolean value indicating whether to encode the first frame only for animated image during encoding. (NSNumber). If not provide, encode animated image if need.
// @note works for `SDImageCoder`.
// 一个布尔值，表示在编码期间是否仅对动画图像编码第一帧。如果不提供，请在需要时编码动画图像。
// 注意：适用于 `SDImageCoder`。
FOUNDATION_EXPORT SDImageCoderOption _Nonnull const SDImageCoderEncodeFirstFrameOnly;

// A double value between 0.0-1.0 indicating the encode compression quality to produce the image data. 1.0 resulting in no compression and 0.0 resulting in the maximum compression possible. If not provide, use 1.0. (NSNumber)
// @note works for `SDImageCoder`
// 0.0-1.0 之间的双倍值，表示生成图像数据的编码压缩质量。1.0 为无压缩，0.0 为可能的最大压缩。如果不提供，默认使用 1.0。
// 注意：适用于 `SDImageCoder`。
FOUNDATION_EXPORT SDImageCoderOption _Nonnull const SDImageCoderEncodeCompressionQuality;

// A SDWebImageContext object which hold the original context options from top-level API. (SDWebImageContext)
// This option is ignored for all built-in coders and take no effect.
// But this may be useful for some custom coders, because some business logic may dependent on things other than image or image data inforamtion only.
// See `SDWebImageContext` for more detailed information.
// SDWebImageContext 对象，用于保存来自 top-level API 的原始上下文选项。(SDWebImageContext)
// 对于所有内置编码器，此选项都将被忽略，并且不起作用。
// 但这可能对某些自定义编码器有用，因为某些业务逻辑可能仅依赖于图像或图像数据信息以外的内容。
// 详细信息请参见 `SDWebImageContext`
FOUNDATION_EXPORT SDImageCoderOption _Nonnull const SDImageCoderWebImageContext;

#pragma mark - Coder

// This is the image coder protocol to provide custom image decoding/encoding.
// These methods are all required to implement.
// @note Pay attention that these methods are not called from main queue.
// 这是提供自定义图像解码/编码的图像编码器协议。
// 这些方法都需要实现。
// 注意：不要从主队列调用这些方法。
@protocol SDImageCoder <NSObject>

@required
#pragma mark - Decoding

// Returns YES if this coder can decode some data. Otherwise, the data should be passed to another coder.
// 如果这个 coder 可以解码 data，则返回 YES。否则，data 应传递给另一个 coder。
- (BOOL)canDecodeFromData:(nullable NSData *)data;

// Decode the image data to image.
// @note This protocol may supports decode animated image frames. You can use `+[SDImageCoderHelper animatedImageWithFrames:]` to produce an animated image with frames.
// 将图像 data 解码成图像。
// 注意：该协议支持解码动画图像帧。您可以使用 `+[SDImageCoderHelper animatedImageWithFrames:]` 生成带帧的动画图像。
- (nullable UIImage *)decodedImageWithData:(nullable NSData *)data
                                   options:(nullable SDImageCoderOptions *)options;

#pragma mark - Encoding

// Returns YES if this coder can encode some image. Otherwise, it should be passed to another coder.
// For custom coder which introduce new image format, you'd better define a new `SDImageFormat` using like this. If you're creating public coder plugin for new image format, also update `https://github.com/rs/SDWebImage/wiki/Coder-Plugin-List` to avoid same value been defined twice.
// 如果这个 coder 可以对某些图像进行编码，则返回 YES。否则，它应该传递给另一个 coder。
// 对于引入新图像格式的自定义 coder，最好使用这样的方法定义一个新的 `SDImageFormat`。如果要为新图像格式创建公共 coder 插件，请同时更新 `https://github.com/rs/SDWebImage/wiki/Coder-Plugin-List`，以避免定义两次相同的值。

// static const SDImageFormat SDImageFormatHEIF = 10;
- (BOOL)canEncodeToFormat:(SDImageFormat)format NS_SWIFT_NAME(canEncode(to:));

// Encode the image to image data.
// @note This protocol may supports encode animated image frames. You can use `+[SDImageCoderHelper framesFromAnimatedImage:]` to assemble an animated image with frames.
// 将图像编码为图像数据。
// 注意：该协议支持编码动画图像帧。您可以使用 `+[SDImageCoderHelper framesFromAnimatedImage:]` 生成带帧的动画图像。
- (nullable NSData *)encodedDataWithImage:(nullable UIImage *)image
                                   format:(SDImageFormat)format
                                  options:(nullable SDImageCoderOptions *)options;

@end

#pragma mark - Progressive Coder

// This is the image coder protocol to provide custom progressive image decoding.
// These methods are all required to implement.
// @note Pay attention that these methods are not called from main queue.
// 这是图像 coder 协议，提供自定义渐进式图像解码。
// 这些方法都需要实现。
// 注意：不要从主队列调用这些方法。
@protocol SDProgressiveImageCoder <SDImageCoder>

@required

// Returns YES if this coder can incremental decode some data. Otherwise, it should be passed to another coder.
// 如果这个 coder 可以渐进式解码某些数据，则返回 YES。否则，它应该传递给另一个 coder。
- (BOOL)canIncrementalDecodeFromData:(nullable NSData *)data;

/**
 Because incremental decoding need to keep the decoded context, we will alloc a new instance with the same class for each download operation to avoid conflicts
 This init method should not return nil

 @param options A dictionary containing any progressive decoding options (instance-level). Pass @{SDImageCoderDecodeScaleFactor: @(1.0)} to specify scale factor for progressive animated image (each frames should use the same scale).
 @return A new instance to do incremental decoding for the specify image format
 */
- (nonnull instancetype)initIncrementalWithOptions:(nullable SDImageCoderOptions *)options;

/**
 Update the incremental decoding when new image data available

 @param data The image data has been downloaded so far
 @param finished Whether the download has finished
 */
- (void)updateIncrementalData:(nullable NSData *)data finished:(BOOL)finished;

/**
 Incremental decode the current image data to image.
 @note Due to the performance issue for progressive decoding and the integration for image view. This method may only return the first frame image even if the image data is animated image. If you want progressive animated image decoding, conform to `SDAnimatedImageCoder` protocol as well and use `animatedImageFrameAtIndex:` instead.

 @param options A dictionary containing any progressive decoding options. Pass @{SDImageCoderDecodeScaleFactor: @(1.0)} to specify scale factor for progressive image
 @return The decoded image from current data
 */
- (nullable UIImage *)incrementalDecodedImageWithOptions:(nullable SDImageCoderOptions *)options;

@end

#pragma mark - Animated Image Provider

// This is the animated image protocol to provide the basic function for animated image rendering. It's adopted by `SDAnimatedImage` and `SDAnimatedImageCoder`
// 这是动画图像协议，为动画图像渲染提供基本功能。它被 `SDAnimatedImage` 和 `SDAnimatedImageCoder` 采用
@protocol SDAnimatedImageProvider <NSObject>

@required

// The original animated image data for current image. If current image is not an animated format, return nil.
// We may use this method to grab back the original image data if need, such as NSCoding or compare.
// 当前图像的原始动画图像数据。如果当前图像不是动画格式，则返回 nil。
// 如果需要，我们可以使用这种方法来获取原始图像数据，例如 NSCoding 或 compare。
@property (nonatomic, copy, readonly, nullable) NSData *animatedImageData;

// Total animated frame count.
// If the frame count is less than 1, then the methods below will be ignored.
// 动画帧数。
// 如果帧数小于 1，则将忽略下面的方法。
@property (nonatomic, assign, readonly) NSUInteger animatedImageFrameCount;

// Animation loop count, 0 means infinite looping.
// 动画循环数，0 表示无限循环。
@property (nonatomic, assign, readonly) NSUInteger animatedImageLoopCount;

// Returns the frame image from a specified index.
// @note The index maybe randomly if one image was set to different imageViews, keep it re-entrant. (It's not recommend to store the images into array because it's memory consuming)
// 返回指定索引中的帧图像。
// 注意：索引可能是随机的，如果一个图像被设置为不同的 imageViews，请将其保存为可重入。（不建议将图像存储到数组中，因为它耗费内存）
- (nullable UIImage *)animatedImageFrameAtIndex:(NSUInteger)index;

// Returns the frames's duration from a specified index.
// @note The index maybe randomly if one image was set to different imageViews, keep it re-entrant. (It's recommend to store the durations into array because it's not memory-consuming)
// 从指定的索引返回帧的持续时间。
// 注意：索引可能是随机的，如果一个图像被设置为不同的 imageViews，请将其保存为可重入。（建议将持续时间存储到数组中，因为它不占用内存）
- (NSTimeInterval)animatedImageDurationAtIndex:(NSUInteger)index;

@end

#pragma mark - Animated Coder

// This is the animated image coder protocol for custom animated image class like  `SDAnimatedImage`. Through it inherit from `SDImageCoder`. We currentlly only use the method `canDecodeFromData:` to detect the proper coder for specify animated image format.
// 这是自定义动画图像类的动画图像 coder 协议，如 `SDAnimatedImage`。继承自 `SDImageCoder`。我们目前只使用 `canDecodeFromData:` 方法来检测指定动画图像格式的正确编码器。
@protocol SDAnimatedImageCoder <SDImageCoder, SDAnimatedImageProvider>

@required

// Because animated image coder should keep the original data, we will alloc a new instance with the same class for the specify animated image data
// The init method should return nil if it can't decode the specify animated image data to produce any frame.
// After the instance created, we may call methods in `SDAnimatedImageProvider` to produce animated image frame.
// return A new instance to do animated decoding for specify image data
// 由于动画图像 coder 应该保留原始数据，因此我们将为指定的动画图像数据分配一个具有相同类的新实例
// init 方法如果无法解码指定的动画图像数据以生成任何帧，则应返回 nil。
// 创建实例后，可以调用 `SDAnimatedImageProvider` 中的方法生成动画图像帧。
// 返回一个新实例，对指定的图像数据进行动画解码
- (nullable instancetype)initWithAnimatedImageData:(nullable NSData *)data options:(nullable SDImageCoderOptions *)options;

@end
