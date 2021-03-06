/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageDefine.h"
#import "SDWebImageManager.h"
#import "SDWebImageTransition.h"
#import "SDWebImageIndicator.h"

// The value specify that the image progress unit count cannot be determined because the progressBlock is not been called.
// 无法确定图像进度的单位计数，因为 progressBlock 没有被调用。
FOUNDATION_EXPORT const int64_t SDWebImageProgressUnitCountUnknown; /* 1LL */

typedef void(^SDSetImageBlock)(UIImage * _Nullable image, NSData * _Nullable imageData, SDImageCacheType cacheType, NSURL * _Nullable imageURL);

// Integrates SDWebImage async downloading and caching of remote images with UIView subclass.
// 将 SDWebImage 异步下载和缓存远程图像与 UIView 子类集成在一起。
@interface UIView (WebCache)

// Get the current image URL.
// @note Note that because of the limitations of categories this property can get out of sync if you use setImage: directly.
// 获取当前图像的 URL
// 注意：由于类别的限制，如果直接使用 setImage，此属性可能会失去同步。
@property (nonatomic, strong, readonly, nullable) NSURL *sd_imageURL;

// The current image loading progress associated to the view. The unit count is the received size and excepted size of download.
// The `totalUnitCount` and `completedUnitCount` will be reset to 0 after a new image loading start (change from current queue). And they will be set to `SDWebImageProgressUnitCountUnknown` if the progressBlock not been called but the image loading success to mark the progress finished (change from main queue).
// @note You can use Key-Value Observing on the progress, but you should take care that the change to progress is from a background queue during download(the same as progressBlock). If you want to using KVO and update the UI, make sure to dispatch on the main queue. And it's recommand to use some KVO libs like KVOController because it's more safe and easy to use.
// @note The getter will create a progress instance if the value is nil. But by default, we don't create one. If you need to use Key-Value Observing, you must trigger the getter or set a custom progresss instance before the loading start. The default value is nil.
// @note Note that because of the limitations of categories this property can get out of sync if you update the progress directly.
// 与视图关联的当前图像加载进度。单位计数是已接收到的大小和除此之外的下载大小。
// `totalUnitCount` 和 `completedUnitCount` 将在新图像加载开始后重置为 0（change from current queue）。如果没有调用 progressBlock，但图像加载成功，则进度标记为已完成（change from main queue），则它们将设置为 `SDWebImageProgressUnitCountUnknown`。
// 注意：你可以在进度上使用 Key-Value Observing，但是应该注意，在下载过程中对进度的更改来自后台队列（the same as progressBlock）。如果要使用 KVO 并更新 UI，请确保在主队列上进行调度。建议使用一些 KVO libs，比如 KVOController，因为它更安全，更容易使用。
// 注意：如果值为 nil，getter 将创建一个进度实例。但通常我们不会创建。如果需要使用 Key-Value Observing，则必须在加载开始之前触发 getter 或设置自定义 progresss 实例。默认值为 nil。
// 注意：由于类别的限制，如果直接更新 progress，此属性可能会失去同步。
@property (nonatomic, strong, null_resettable) NSProgress *sd_imageProgress;

// Set the imageView `image` with an `url` and optionally a placeholder image.
// The download is asynchronous and cached.
// 使用 `url` 和可选的占位符图像设置 imageView 的 `image`。
// 下载是异步的并且缓存。

// setImageBlock: Block used for custom set image code. If not provide, use the built-in set image code (supports `UIImageView/NSImageView` and `UIButton/NSButton` currently)
// setImageBlock: 用于自定义设置图像代码的 block。如果未提供，则使用内置的设置图像代码（当前支持 `UIImageView/NSImageView` 和 `UIButton/NSButton`）。

// completedBlock: This block has no return value and takes the requested UIImage as first parameter and the NSData representation as second parameter. In case of error the image parameter is nil and the third parameter may contain an NSError.
// The forth parameter is an `SDImageCacheType` enum indicating if the image was retrieved from the local cache or from the memory cache or from the network.
// The fith parameter normally is always YES. However, if you provide SDWebImageAvoidAutoSetImage with SDWebImageProgressiveLoad options to enable progressive downloading and set the image yourself. This block is thus called repeatedly with a partial image. When image is fully downloaded, the block is called a last time with the full image and the last parameter set to YES.
// The last parameter is the original image URL
// completedBlock: 这个 block 没有返回值，将请求的 UIImage 作为第一个参数，NSData 作为第二个参数。如果出现错误，image 参数为 nil，第三个参数可能包含 NSError。
// 第四个参数是 `SDImageCacheType` 枚举，指示图像是从本地缓存还是从内存缓存或网络中检索的。
// 第五个参数通常为 YES。但是，如果你为 SDWebImageAvoidAutoSetImage 提供 SDWebImageProgressiveLoad 选项以启用渐进式下载并自己设置图像。因此，部分图像会重复调用这个 block。当图像完全下载时，这个 block 将在最后一次调用时将完整图像和最后一个参数设置为 YES。
// 最后一个参数是原始图像 URL。
- (void)sd_internalSetImageWithURL:(nullable NSURL *)url
                  placeholderImage:(nullable UIImage *)placeholder
                           options:(SDWebImageOptions)options
                           context:(nullable SDWebImageContext *)context
                     setImageBlock:(nullable SDSetImageBlock)setImageBlock
                          progress:(nullable SDImageLoaderProgressBlock)progressBlock
                         completed:(nullable SDInternalCompletionBlock)completedBlock;

// Cancel the current image load
// 取消当前图像加载
- (void)sd_cancelCurrentImageLoad;

#if SD_UIKIT || SD_MAC

#pragma mark - Image Transition

// The image transition when image load finished. See `SDWebImageTransition`.
// If you specify nil, do not do transition. Defautls to nil.
// 图像加载完成时的图像转换。请参见 `SDWebImageTransition`。如果指定 nil，则不要进行转换。通常为 nil。
@property (nonatomic, strong, nullable) SDWebImageTransition *sd_imageTransition;

#pragma mark - Image Indicator

/**
 The image indicator during the image loading. If you do not need indicator, specify nil. Defaults to nil
 The setter will remove the old indicator view and add new indicator view to current view's subview.
 @note Because this is UI related, you should access only from the main queue.
 */
// 图像加载期间的图像指示器。如果不需要指示器，请指定 nil。默认为 nil。
// setter 将移除旧的指示器 view，并将新的指示器 view 添加到当前 view 的子视图中。
// 注意：因为这与 UI 相关，所以只能从主队列访问。
@property (nonatomic, strong, nullable) id<SDWebImageIndicator> sd_imageIndicator;

#endif

@end
