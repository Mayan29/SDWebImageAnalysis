/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

typedef void(^SDWebImageNoParamsBlock)(void);
typedef NSString * SDWebImageContextOption NS_EXTENSIBLE_STRING_ENUM;
typedef NSDictionary<SDWebImageContextOption, id> SDWebImageContext;
typedef NSMutableDictionary<SDWebImageContextOption, id> SDWebImageMutableContext;

#pragma mark - Image scale

/**
 Return the image scale factor for the specify key, supports file name and url key.
 This is the built-in way to check the scale factor when we have no context about it. Because scale factor is not stored in image data (It's typically from filename).
 However, you can also provide custom scale factor as well, see `SDWebImageContextImageScaleFactor`.

 @param key The image cache key
 @return The scale factor for image
 */
FOUNDATION_EXPORT CGFloat SDImageScaleFactorForKey(NSString * _Nullable key);

/**
 Scale the image with the scale factor for the specify key. If no need to scale, return the original image.
 This works for `UIImage`(UIKit) or `NSImage`(AppKit). And this function also preserve the associated value in `UIImage+Metadata.h`.
 @note This is actually a convenience function, which firstlly call `SDImageScaleFactorForKey` and then call `SDScaledImageForScaleFactor`, kept for backward compatibility.

 @param key The image cache key
 @param image The image
 @return The scaled image
 */
FOUNDATION_EXPORT UIImage * _Nullable SDScaledImageForKey(NSString * _Nullable key, UIImage * _Nullable image);

/**
 Scale the image with the scale factor. If no need to scale, return the original image.
 This works for `UIImage`(UIKit) or `NSImage`(AppKit). And this function also preserve the associated value in `UIImage+Metadata.h`.
 
 @param scale The image scale factor
 @param image The image
 @return The scaled image
 */
FOUNDATION_EXPORT UIImage * _Nullable SDScaledImageForScaleFactor(CGFloat scale, UIImage * _Nullable image);

#pragma mark - WebCache Options

/// WebCache options
/// 缓存选项
typedef NS_OPTIONS(NSUInteger, SDWebImageOptions) {
    
    // By default, when a URL fail to be downloaded, the URL is blacklisted so the library won't keep trying.
    // This flag disable this blacklisting.
    // 默认情况下，下载失败的时候 URL 会被列入黑名单，因此不会再次尝试下载。此标识会阻止 library 把失败的 url 加入黑名单。
    SDWebImageRetryFailed = 1 << 0,
    
    // By default, image downloads are started during UI interactions, this flags disable this feature, leading to delayed download on UIScrollView deceleration for instance.
    // 默认情况下，图像会在 UI 交互期间下载。此标识禁用此功能，使其延迟到 UIScrollView 减速的时候才开始下载。
    SDWebImageLowPriority = 1 << 1,
    
    // This flag enables progressive download, the image is displayed progressively during download as a browser would do.
    // By default, the image is only displayed once completely downloaded.
    // 默认情况下，图像仅在完全下载后显示。此标识启用渐进式下载，图像在下载期间逐步显示。
    SDWebImageProgressiveLoad = 1 << 2,
    
    // Even if the image is cached, respect the HTTP response cache control, and refresh the image from remote location if needed.
    // The disk caching will be handled by NSURLCache instead of SDWebImage leading to slight performance degradation.
    // This option helps deal with images changing behind the same request URL, e.g. Facebook graph api profile pics.
    // If a cached image is refreshed, the completion block is called once with the cached image and again with the final image.
    // Use this flag only if you can't make your URLs static with embedded cache busting parameter.
    // 即使图像缓存，也要遵守 HTTP 响应缓存，并在需要时从远程位置刷新图像。
    // 磁盘缓存将由 NSURLCache 而不是 SDWebImage 处理，从而导致轻微的性能下降。
    // 此选项有助于处理在 URL 不变的情况下更改的图像，例如 Facebook 图形 api 配置文件图片。
    // 如果刷新缓存的图像，则使用缓存的图像调用完成 block，并再次使用最终图像调用完成 block。
    // 使用此标识，如果你想 URL 不变时刷新缓存。
    SDWebImageRefreshCached = 1 << 3,
    
    // In iOS 4+, continue the download of the image if the app goes to background. This is achieved by asking the system for extra time in background to let the request finish. If the background task expires the operation will be cancelled.
    // 在 iOS 4+ 版本，APP 进入后台继续下载图像。这是通过向系统索要后台的额外时间以完成请求来实现的。如果后台任务到期，则操作将被取消。
    SDWebImageContinueInBackground = 1 << 4,
    
    // Handles cookies stored in NSHTTPCookieStore by setting NSMutableURLRequest.HTTPShouldHandleCookies = YES;
    // 通过设置 NSMutableURLRequest.HTTPShouldHandleCookies = YES; 控制 cookies 存储到 NSHTTPCookieStore
    SDWebImageHandleCookies = 1 << 5,
    
    // Enable to allow untrusted SSL certificates.
    // Useful for testing purposes. Use with caution in production.
    // 允许不受信任的 SSL 证书。用于测试，在生产中谨用。
    SDWebImageAllowInvalidSSLCertificates = 1 << 6,
    
    // By default, images are loaded in the order in which they were queued. This flag moves them to the front of the queue.
    // 默认情况下，图像按其排队顺序下载。此标识将它们优先下载。
    SDWebImageHighPriority = 1 << 7,
    
    // By default, placeholder images are loaded while the image is loading. This flag will delay the loading of the placeholder image until after the image has finished loading.
    // 默认情况下，在加载图像时加载 placeholder 图像。此标识将延迟加载 placeholder 图像，直到图像完成加载。
    SDWebImageDelayPlaceholder = 1 << 8,
    
    // We usually don't apply transform on animated images as most transformers could not manage animated images.
    // Use this flag to transform them anyway.
    // 我们通常不对动画图像采用 transform，因为大多数变换器无法管理动画图像。使用此标识，无论如何都要使用。
    SDWebImageTransformAnimatedImage = 1 << 9,
    
    // By default, image is added to the imageView after download. But in some cases, we want to have the hand before setting the image (apply a filter or add it with cross-fade animation for instance)
    // Use this flag if you want to manually set the image in the completion when success
    // 默认情况下，下载后将图像加载到 imageView。但在某些情况下，我们希望在加载图像之前处理一下图像（例如添加滤镜或者添加透明动画）。
    // 使用此标识，在下载图像之后手动设置图像
    SDWebImageAvoidAutoSetImage = 1 << 10,
    
    // By default, images are decoded respecting their original size. On iOS, this flag will scale down the images to a size compatible with the constrained memory of devices.
    // This flag take no effect if `SDWebImageAvoidDecodeImage` is set. And it will be ignored if `SDWebImageProgressiveLoad` is set.
    // 默认情况下，图像会根据其原始大小进行解码。使用此标识，会将图像缩小到与设备的受限内存兼容的大小。
    // 如果设置了「SDWebImageAvoidDecodeImage」此标识不起作用。如果设置了「SDWebImageProgressiveLoad」它将被 ignored。
    SDWebImageScaleDownLargeImages = 1 << 11,
    
    // By default, we do not query image data when the image is already cached in memory. This mask can force to query image data at the same time. However, this query is asynchronously unless you specify `SDWebImageQueryMemoryDataSync`
    // 默认情况下，当图像已缓存在内存中时，我们不会查询图像数据。
    // 使用此标识，可以同时强制查询图像数据。但是，除非指定「SDWebImageQueryMemoryDataSync」否则此查询是异步的。
    SDWebImageQueryMemoryData = 1 << 12,
    
    // By default, when you only specify `SDWebImageQueryMemoryData`, we query the memory image data asynchronously. Combined this mask as well to query the memory image data synchronously.
    // @note Query data synchronously is not recommend, unless you want to ensure the image is loaded in the same runloop to avoid flashing during cell reusing.
    // 默认情况下，当您只指定「SDWebImageQueryMemoryData」时，我们异步查询内存中的图像数据。使用此标识，同步查询图像数据。
    // 一般不建议同步查询数据，除非你希望确保在同一个 runloop 中加载图像以避免在 cell 重用时闪烁。
    SDWebImageQueryMemoryDataSync = 1 << 13,
    
    // By default, when the memory cache miss, we query the disk cache asynchronously. This mask can force to query disk cache (when memory cache miss) synchronously.
    // @note These 3 query options can be combined together. For the full list about these masks combination, see wiki page.
    // @note Query data synchronously is not recommend, unless you want to ensure the image is loaded in the same runloop to avoid flashing during cell reusing.
    // 默认情况下，当内存缓存丢失时，我们会异步查询磁盘缓存。使用此标识，当内存缓存丢失时，可以强制同步查询磁盘缓存。
    // 这 3 个查询选项可以组合在一起，详情请参阅 Wiki 页面。
    // 同样不建议同步查询数据，除非你希望确保在同一个 runloop 中加载图像以避免在 cell 重用时闪烁。
    SDWebImageQueryDiskDataSync = 1 << 14,

    // By default, when the cache missed, the image is load from the loader. This flag can prevent this to load from cache only.
    // 默认情况下，当内存缓存丢失时，图像从 loader 加载。使用此标识，仅从内存缓存中加载。
    SDWebImageFromCacheOnly = 1 << 15,
    
    // By default, we query the cache before the image is load from the loader. This flag can prevent this to load from loader only.
    // 默认情况下，我们在从 loader 加载图像之前查询缓存。使用此标识，可以防止仅从 loader 加载。
    SDWebImageFromLoaderOnly = 1 << 16,

    // By default, when you use `SDWebImageTransition` to do some view transition after the image load finished, this transition is only applied for image download from the network. This mask can force to apply view transition for memory and disk cache as well.
    // 默认情况下，在图像加载完成后使用「SDWebImageTransition」进行某些视图转换时，此转换仅适用于从网络下载的图像。
    // 使用此标识，可以强制为内存和磁盘缓存提供视图转换。
    SDWebImageForceTransition = 1 << 17,
    
    // By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
    // However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
    // 默认情况下，在查询内存和从网络下载期间，我们将在后台解码图像。这有助于提高性能，因为在屏幕上渲染图像时，需要首先对其进行解码。此过程发生在 Core Animation 的主队列中。
    // 然而，此过程也可能会增加内存使用量。如果由于过多的内存消耗而遇到问题，此标志可以防止解码图像。
    SDWebImageAvoidDecodeImage = 1 << 18,
    
    // By default, we decode the animated image. This flag can force decode the first frame only and produece the static image.
    // 默认情况下，我们解码动画图像。使用此标识，只能强制解码第一帧并生成静态图像。
    SDWebImageDecodeFirstFrameOnly = 1 << 19,
    
    // By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. However, you can specify to preload all frames into memory to reduce CPU usage when the animated image is shared by lots of imageViews.
    // This will actually trigger `preloadAllAnimatedImageFrames` in the background queue(Disk Cache & Download only).
    // 默认情况下，对于「SDAnimatedImage」我们在渲染过程中解码动画图像 frame，以减少内存使用量。但是，你可以将所有 frame 预加载到内存中，以被大量 imageView 共享动画图像时降低 CPU 使用率。
    // 这实际上会在后台队列中触发「preloadAllAnimatedImageFrames」（仅限磁盘缓存和下载）。
    SDWebImagePreloadAllFrames = 1 << 20,
    
    // By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available to produce one exactlly matching your custom class as a fallback solution.
    // Using this option, can ensure we always callback image with your provided class. If failed to produce one, a error with code `SDWebImageErrorBadImageData` will been used.
    // Note this options is not compatible with `SDWebImageDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
    // 默认情况下，当你使用「SDWebImageContextAnimatedImageClass」上下文选项（例如使用「SDAnimatedImageView」设计使用「SDAnimatedImage」）时
    // 我们仍然可以在内存缓存命中时使用「UIImage」，或者图像解码器无法生成一个完全匹配你自定义 class 作为备选方案。
    // 使用此选项，可以确保我们始终使用您提供的 class 回调图像。如果生成失败，将使用「SDWebImageErrorBadImageData」报错。
    // 注意，此选项与「SDWebImageDecodeFirstFrameOnly」不兼容，后者始终生成 UIImage / NSImage。
    SDWebImageMatchAnimatedImageClass = 1 << 21,
};


#pragma mark - Context Options

/**
 A String to be used as the operation key for view category to store the image load operation. This is used for view instance which supports different image loading process. If nil, will use the class name as operation key. (NSString *)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextSetImageOperationKey;

/**
 A SDWebImageManager instance to control the image download and cache process using in UIImageView+WebCache category and likes. If not provided, use the shared manager (SDWebImageManager *)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCustomManager;

/**
 A id<SDImageTransformer> instance which conforms `SDImageTransformer` protocol. It's used for image transform after the image load finished and store the transformed image to cache. If you provide one, it will ignore the `transformer` in manager and use provided one instead. (id<SDImageTransformer>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageTransformer;

/**
 A CGFloat raw value which specify the image scale factor. The number should be greater than or equal to 1.0. If not provide or the number is invalid, we will use the cache key to specify the scale factor. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextImageScaleFactor;

/**
 A SDImageCacheType raw value which specify the store cache type when the image has just been downloaded and will be stored to the cache. Specify `SDImageCacheTypeNone` to disable cache storage; `SDImageCacheTypeDisk` to store in disk cache only; `SDImageCacheTypeMemory` to store in memory only. And `SDImageCacheTypeAll` to store in both memory cache and disk cache.
 If you use image transformer feature, this actually apply for the transformed image, but not the original image itself. Use `SDWebImageContextOriginalStoreCacheType` if you want to control the original image's store cache type at the same time.
 If not provide or the value is invalid, we will use `SDImageCacheTypeAll`. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextStoreCacheType;

/**
 The same behavior like `SDWebImageContextStoreCacheType`, but control the store cache type for the original image when you use image transformer feature. This allows the detail control of cache storage for these two images. For example, if you want to store the transformed image into both memory/disk cache, store the original image into disk cache only, use `[.storeCacheType : .all, .originalStoreCacheType : .disk]`
 If not provide or the value is invalid, we will use `SDImageCacheTypeNone`, which does not store the original image into cache. (NSNumber)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextOriginalStoreCacheType;

/**
 A Class object which the instance is a `UIImage/NSImage` subclass and adopt `SDAnimatedImage` protocol. We will call `initWithData:scale:options:` to create the instance (or `initWithAnimatedCoder:scale:` when using progressive download) . If the instance create failed, fallback to normal `UIImage/NSImage`.
 This can be used to improve animated images rendering performance (especially memory usage on big animated images) with `SDAnimatedImageView` (Class).
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextAnimatedImageClass;

/**
 A id<SDWebImageDownloaderRequestModifier> instance to modify the image download request. It's used for downloader to modify the original request from URL and options. If you provide one, it will ignore the `requestModifier` in downloader and use provided one instead. (id<SDWebImageDownloaderRequestModifier>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextDownloadRequestModifier;

/**
 A id<SDWebImageCacheKeyFilter> instance to convert an URL into a cache key. It's used when manager need cache key to use image cache. If you provide one, it will ignore the `cacheKeyFilter` in manager and use provided one instead. (id<SDWebImageCacheKeyFilter>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCacheKeyFilter;

/**
 A id<SDWebImageCacheSerializer> instance to convert the decoded image, the source downloaded data, to the actual data. It's used for manager to store image to the disk cache. If you provide one, it will ignore the `cacheSerializer` in manager and use provided one instead. (id<SDWebImageCacheSerializer>)
 */
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextCacheSerializer;
