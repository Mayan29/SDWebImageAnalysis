/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDWebImageDefine.h"
#import "SDWebImageOperation.h"
#import "SDWebImageDownloaderConfig.h"
#import "SDWebImageDownloaderRequestModifier.h"
#import "SDImageLoader.h"

/// Downloader options
typedef NS_OPTIONS(NSUInteger, SDWebImageDownloaderOptions) {
    
    // Put the download in the low queue priority and task priority.
    // 将下载放入低优先级的队列和任务中。
    SDWebImageDownloaderLowPriority = 1 << 0,
    
    // This flag enables progressive download, the image is displayed progressively during download as a browser would do.
    // 此标识启用渐进式下载，图像在下载过程中逐步显示，就像浏览器一样。
    SDWebImageDownloaderProgressiveLoad = 1 << 1,

    // By default, request prevent the use of NSURLCache. With this flag, NSURLCache is used with default policies.
    // 默认情况下，请求会阻止使用 NSURLCache。使用此标识，默认使用 NSURLCache。
    SDWebImageDownloaderUseNSURLCache = 1 << 2,

    // Call completion block with nil image/imageData if the image was read from NSURLCache
    // And the error code is `SDWebImageErrorCacheNotModified`
    // This flag should be combined with `SDWebImageDownloaderUseNSURLCache`.
    // 如果从 NSURLCache 读取图像，则使用 nil image/imageData 调用完成 block
    // 错误代码是 `SDWebImageErrorCacheNotModified`
    // 此标识应与 `SDWebImageDownloaderUseNSURLCache` 结合使用。
    SDWebImageDownloaderIgnoreCachedResponse = 1 << 3,
    
    // In iOS 4+, continue the download of the image if the app goes to background. This is achieved by asking the system for extra time in background to let the request finish. If the background task expires the operation will be cancelled.
    // 在 iOS4+，如果 app 转到后台，继续下载图像。这是通过在后台请求系统额外的时间来实现的。如果后台任务过期，则操作将被取消。
    SDWebImageDownloaderContinueInBackground = 1 << 4,

    // Handles cookies stored in NSHTTPCookieStore by setting NSMutableURLRequest.HTTPShouldHandleCookies = YES;
    // 通过设置 NSMutableURLRequest.HTTPShouldHandleCookies = YES; 来处理存储在 NSHTTPCookieStore 中的 cookie
    SDWebImageDownloaderHandleCookies = 1 << 5,

    // Enable to allow untrusted SSL certificates.
    // Useful for testing purposes. Use with caution in production.
    // 允许不受信任的 SSL 证书。
    // 用于测试，在生产中小心使用。
    SDWebImageDownloaderAllowInvalidSSLCertificates = 1 << 6,

    // Put the download in the high queue priority and task priority.
    // 将下载放入高优先级的队列和任务中。
    SDWebImageDownloaderHighPriority = 1 << 7,
    
    // By default, images are decoded respecting their original size. On iOS, this flag will scale down the images to a size compatible with the constrained memory of devices.
    // This flag take no effect if `SDWebImageDownloaderAvoidDecodeImage` is set. And it will be ignored if `SDWebImageDownloaderProgressiveLoad` is set.
    // 默认情况下，图像根据其原始大小进行解码。在 iOS 中，此标识将缩小图像的大小，使其与受限制的设备内存兼容。
    // 如果设置了 `SDWebImageDownloaderAvoidDecodeImage`，则此标识无效。如果设置了 `SDWebImageDownloaderProgressiveLoad`，则此标识被忽略。
    SDWebImageDownloaderScaleDownLargeImages = 1 << 8,
    
    // By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
    // However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
    // 默认情况下，在缓存查询和网络下载时，我们会在后台解码图像。这有助于提高性能，因为在屏幕上渲染图像时，需要首先对其进行解码。但这发生在主队列上的核心动画。
    // 但是，这个过程也可能增加内存使用量。如果由于内存消耗过多而遇到问题，则此标识可以阻止对图像进行解码。
    SDWebImageDownloaderAvoidDecodeImage = 1 << 9,
    
    // By default, we decode the animated image. This flag can force decode the first frame only and produece the static image.
    // 默认情况下，我们解码动画图像。此标识只能强制解码第一帧并生成静态图像。
    SDWebImageDownloaderDecodeFirstFrameOnly = 1 << 10,
    
    // By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. This flag actually trigger `preloadAllAnimatedImageFrames = YES` after image load from network
    // 默认情况下，对于 `SDAnimatedImage`，我们在渲染期间解码动画图像帧以减少内存使用。此标识实际上在从网络加载图像后触发 `preloadAllAnimatedImageFrames = YES`
    SDWebImageDownloaderPreloadAllFrames = 1 << 11,
    
    // By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available, to behave as a fallback solution.
    // Using this option, can ensure we always produce image with your provided class. If failed, a error with code `SDWebImageErrorBadImageData` will been used.
    // Note this options is not compatible with `SDWebImageDownloaderDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
    // 默认情况下，当你使用 `SDWebImageContextAnimatedImageClass` 上下文选项时（类似于使用 `SDAnimatedImageView`，该视图设计为使用 `SDAnimatedImage`），当内存缓存命中或图像解码器不可用时，我们仍然可以使用 `UIImage` 作为后备解决方案。
    // 使用此标识，我们可以保证用您提供的 class 制作图像。如果失败，a error with code `SDWebImageErrorBadImageData` will been used。
    // 注意：此标识与 `SDWebImageDownloaderDecodeFirstFrameOnly` 不兼容，后者总是生成 UIImage/NSImage。
    SDWebImageDownloaderMatchAnimatedImageClass = 1 << 12,
};

FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadStartNotification;
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadReceiveResponseNotification;
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadStopNotification;
FOUNDATION_EXPORT NSNotificationName _Nonnull const SDWebImageDownloadFinishNotification;

typedef SDImageLoaderProgressBlock SDWebImageDownloaderProgressBlock;
typedef SDImageLoaderCompletedBlock SDWebImageDownloaderCompletedBlock;

// A token associated with each download. Can be used to cancel a download
// 与每次下载相关联的 token。可用于取消下载
@interface SDWebImageDownloadToken : NSObject <SDWebImageOperation>

// Cancel the current download.
- (void)cancel;

// The download's URL.
@property (nonatomic, strong, nullable, readonly) NSURL *url;

// The download's request.
@property (nonatomic, strong, nullable, readonly) NSURLRequest *request;

// The download's response.
@property (nonatomic, strong, nullable, readonly) NSURLResponse *response;

@end


// Asynchronous downloader dedicated and optimized for image loading.
// 专门为图像加载而优化的异步下载器。
@interface SDWebImageDownloader : NSObject

// Downloader Config object - storing all kind of settings.
// Most config properties support dynamic changes during download, except something like `sessionConfiguration`, see `SDWebImageDownloaderConfig` for more detail.
// 下载器配置对象 - 存储所有类型的设置。
// 除了 `sessionConfiguration` 之类的 something，大多数配置属性都支持下载期间的动态更改，有关详细信息，请参见 `SDWebImageDownloaderConfig`。
@property (nonatomic, copy, readonly, nonnull) SDWebImageDownloaderConfig *config;

// Set the request modifier to modify the original download request before image load.
// This request modifier method will be called for each downloading image request. Return the original request means no modication. Return nil will cancel the download request.
// Defaults to nil, means does not modify the original download request.
// @note If you want to modify single request, consider using `SDWebImageContextDownloadRequestModifier` context option.
// 设置请求修饰符，在图像加载前修改原始下载请求。
// 每次下载图像请求都会调用此请求修饰符方法。返回原始请求意味着没有修改。返回 nil 将取消下载请求。
// 默认为 nil，表示不修改原始下载请求。
// 注意：如果要修改单个请求，请考虑使用 `SDWebImageContextDownloadRequestModifier` 上下文选项。
@property (nonatomic, strong, nullable) id<SDWebImageDownloaderRequestModifier> requestModifier;

// The configuration in use by the internal NSURLSession. If you want to provide a custom sessionConfiguration, use `SDWebImageDownloaderConfig.sessionConfiguration` and create a new downloader instance.
// @note This is immutable according to NSURLSession's documentation. Mutating this object directly has no effect.
// 内部正在使用的 NSURLSession 的配置。如果要提供自定义 sessionConfiguration，请使用 `SDWebImageDownloaderConfig.sessionConfiguration` 并创建新的 downloader 实例。
// 注意：根据 NSURLSession 文档，这是不可变的。直接改变这个对象没有效果。
@property (nonatomic, readonly, nonnull) NSURLSessionConfiguration *sessionConfiguration;

// Gets/Sets the download queue suspension state.
// Gets/Sets 下载队列挂起状态。
@property (nonatomic, assign, getter=isSuspended) BOOL suspended;

// Shows the current amount of downloads that still need to be downloaded
// 显示当前需要下载的下载数量
@property (nonatomic, assign, readonly) NSUInteger currentDownloadCount;

// Returns the global shared downloader instance. Which use the `SDWebImageDownloaderConfig.defaultDownloaderConfig` config.
// 返回全局共享 downloader 实例。使用 `SDWebImageDownloaderConfig.defaultDownloaderConfig` 配置。
@property (nonatomic, class, readonly, nonnull) SDWebImageDownloader *sharedDownloader;

// Creates an instance of a downloader with specified downloader config.
// You can specify session configuration, timeout or operation class through downloader config.
// config: The downloader config. If you specify nil, the `defaultDownloaderConfig` will be used.
// return new instance of downloader class
// 使用指定的 downloader 配置创建 downloader 实例。
// 可以通过 downloader 配置指定会话配置、超时或操作 class。
// config: downloader 配置。如果指定 nil，则将使用 `defaultDownloaderConfig`。
// 返回 downloader class 的新实例
- (nonnull instancetype)initWithConfig:(nullable SDWebImageDownloaderConfig *)config NS_DESIGNATED_INITIALIZER;

// Set a value for a HTTP header to be appended to each download HTTP request.
// value: The value for the header field. Use `nil` value to remove the header field.
// field: The name of the header field to set.
// 为 HTTP header 设置一个 value，添加到每个下载 HTTP 请求中
// value: 头字段的 value。使用 nil 删除头字段。
// field: 头字段的 key。
- (void)setValue:(nullable NSString *)value forHTTPHeaderField:(nullable NSString *)field;

// Returns the value of the specified HTTP header field.
// @return The value associated with the header field field, or `nil` if there is no corresponding header field.
// 返回指定的 HTTP 头字段的 value。
// 返回与头字段相关联的 value，如果没有对应的头字段，则返回 nil。
- (nullable NSString *)valueForHTTPHeaderField:(nullable NSString *)field;

/**
 * Creates a SDWebImageDownloader async downloader instance with a given URL
 *
 * The delegate will be informed when the image is finish downloaded or an error has happen.
 *
 * @see SDWebImageDownloaderDelegate
 *
 * @param url            The URL to the image to download
 * @param completedBlock A block called once the download is completed.
 *                       If the download succeeded, the image parameter is set, in case of error,
 *                       error parameter is set with the error. The last parameter is always YES
 *                       if SDWebImageDownloaderProgressiveDownload isn't use. With the
 *                       SDWebImageDownloaderProgressiveDownload option, this block is called
 *                       repeatedly with the partial image object and the finished argument set to NO
 *                       before to be called a last time with the full image and finished argument
 *                       set to YES. In case of error, the finished argument is always YES.
 *
 * @return A token (SDWebImageDownloadToken) that can be used to cancel this operation
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * Creates a SDWebImageDownloader async downloader instance with a given URL
 *
 * The delegate will be informed when the image is finish downloaded or an error has happen.
 *
 * @see SDWebImageDownloaderDelegate
 *
 * @param url            The URL to the image to download
 * @param options        The options to be used for this download
 * @param progressBlock  A block called repeatedly while the image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called once the download is completed.
 *                       If the download succeeded, the image parameter is set, in case of error,
 *                       error parameter is set with the error. The last parameter is always YES
 *                       if SDWebImageDownloaderProgressiveLoad isn't use. With the
 *                       SDWebImageDownloaderProgressiveLoad option, this block is called
 *                       repeatedly with the partial image object and the finished argument set to NO
 *                       before to be called a last time with the full image and finished argument
 *                       set to YES. In case of error, the finished argument is always YES.
 *
 * @return A token (SDWebImageDownloadToken) that can be used to cancel this operation
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageDownloaderOptions)options
                                                  progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * Creates a SDWebImageDownloader async downloader instance with a given URL
 *
 * The delegate will be informed when the image is finish downloaded or an error has happen.
 *
 * @see SDWebImageDownloaderDelegate
 *
 * @param url            The URL to the image to download
 * @param options        The options to be used for this download
 * @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param progressBlock  A block called repeatedly while the image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called once the download is completed.
 *
 * @return A token (SDWebImageDownloadToken) that can be used to cancel this operation
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageDownloaderOptions)options
                                                   context:(nullable SDWebImageContext *)context
                                                  progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * Cancels all download operations in the queue
 */
- (void)cancelAllDownloads;

/**
 * Invalidates the managed session, optionally canceling pending operations.
 * @note If you use custom downloader instead of the shared downloader, you need call this method when you do not use it to avoid memory leak
 * @param cancelPendingOperations Whether or not to cancel pending operations.
 * @note Calling this method on the shared downloader has no effect.
 */
- (void)invalidateSessionAndCancel:(BOOL)cancelPendingOperations;

@end


// SDWebImageDownloader is the built-in image loader conform to `SDImageLoader`. Which provide the HTTP/HTTPS/FTP download, or local file URL using NSURLSession.
// However, this downloader class itself also support customization for advanced users. You can specify `operationClass` in download config to custom download operation, See `SDWebImageDownloaderOperation`.
// If you want to provide some image loader which beyond network or local file, consider to create your own custom class conform to `SDImageLoader`.
// SDWebImageDownloader 是遵守 `SDImageLoader` 的内置图像加载器。它使用 NSURLSession 提供 HTTP/HTTPS/FTP 下载或本地文件 URL。
// 然鹅，这个下载器 class 本身也支持为高级用户定制。你可以在下载配置中将 `operationClass` 指定为自定义下载操作，请参见 `SDWebImageDownloaderOperation`。
// 如果要提供一些超出网络或本地文件的图像加载器，请考虑创建符合 `SDImageLoader` 的自定义类。
@interface SDWebImageDownloader (SDImageLoader) <SDImageLoader>

@end
