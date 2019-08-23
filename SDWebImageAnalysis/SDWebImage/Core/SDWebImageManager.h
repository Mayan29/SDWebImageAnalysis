/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"
#import "SDImageCacheDefine.h"
#import "SDImageLoader.h"
#import "SDImageTransformer.h"
#import "SDWebImageCacheKeyFilter.h"
#import "SDWebImageCacheSerializer.h"
#import "SDWebImageOptionsProcessor.h"

// UIImageView+WebCache 中的回调 block
// image: 请求的图像
// error: 错误信息
// cacheType: 图像存储类型（本地缓存还是网络获取）
// imageURL: 原始图像 URL
typedef void(^SDExternalCompletionBlock)(UIImage * _Nullable image, NSError * _Nullable error, SDImageCacheType cacheType, NSURL * _Nullable imageURL);

// UIView+WebCache 中的回调 block
typedef void(^SDInternalCompletionBlock)(UIImage * _Nullable image, NSData * _Nullable data, NSError * _Nullable error, SDImageCacheType cacheType, BOOL finished, NSURL * _Nullable imageURL);

// A combined operation representing the cache and loader operation. You can use it to cancel the load process.
// 表示缓存和加载操作的组合操作。你可以使用它取消加载过程。
@interface SDWebImageCombinedOperation : NSObject <SDWebImageOperation>

/**
 Cancel the current operation, including cache and loader process
 */
- (void)cancel;

/**
 The cache operation from the image cache query
 */
@property (strong, nonatomic, nullable, readonly) id<SDWebImageOperation> cacheOperation;

/**
 The loader operation from the image loader (such as download operation)
 */
@property (strong, nonatomic, nullable, readonly) id<SDWebImageOperation> loaderOperation;

@end


@class SDWebImageManager;

/**
 The manager delegate protocol.
 */
@protocol SDWebImageManagerDelegate <NSObject>

@optional

/**
 * Controls which image should be downloaded when the image is not found in the cache.
 *
 * @param imageManager The current `SDWebImageManager`
 * @param imageURL     The url of the image to be downloaded
 *
 * @return Return NO to prevent the downloading of the image on cache misses. If not implemented, YES is implied.
 */
- (BOOL)imageManager:(nonnull SDWebImageManager *)imageManager shouldDownloadImageForURL:(nonnull NSURL *)imageURL;

/**
 * Controls the complicated logic to mark as failed URLs when download error occur.
 * If the delegate implement this method, we will not use the built-in way to mark URL as failed based on error code;
 @param imageManager The current `SDWebImageManager`
 @param imageURL The url of the image
 @param error The download error for the url
 @return Whether to block this url or not. Return YES to mark this URL as failed.
 */
- (BOOL)imageManager:(nonnull SDWebImageManager *)imageManager shouldBlockFailedURL:(nonnull NSURL *)imageURL withError:(nonnull NSError *)error;

@end

// The SDWebImageManager is the class behind the UIImageView+WebCache category and likes.
// It ties the asynchronous downloader (SDWebImageDownloader) with the image cache store (SDImageCache).
// You can use this class directly to benefit from web image downloading with caching in another context than
// a UIView.
// SDWebImageManager 是 UIImageView+WebCache 类别后面的类。
// 它将异步下载器 SDWebImageDownloader 与图像缓存存储 SDImageCache 联系起来。
// 你可以直接使用这个类从 web 图像下载中获益，在另一个上下文中使用缓存，而不是 UIView。

/**
 * Here is a simple example of how to use SDWebImageManager:
 *
 * @code

SDWebImageManager *manager = [SDWebImageManager sharedManager];
[manager loadImageWithURL:imageURL
                  options:0
                 progress:nil
                completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, BOOL finished, NSURL *imageURL) {
                    if (image) {
                        // do something with image
                    }
                }];

 * @endcode
 */
@interface SDWebImageManager : NSObject

/**
 * The delegate for manager. Defatuls to nil.
 */
@property (weak, nonatomic, nullable) id <SDWebImageManagerDelegate> delegate;

/**
 * The image cache used by manager to query image cache.
 */
@property (strong, nonatomic, readonly, nonnull) id<SDImageCache> imageCache;

/**
 * The image loader used by manager to load image.
 */
@property (strong, nonatomic, readonly, nonnull) id<SDImageLoader> imageLoader;

// The image transformer for manager. It's used for image transform after the image load finished and store the transformed image to cache, see `SDImageTransformer`.
// Defaults to nil, which means no transform is applied.
// @note This will affect all the load requests for this manager if you provide. However, you can pass `SDWebImageContextImageTransformer` in context arg to explicitly use that transformer instead.
// manager 的图像转换器。它用于图像加载完成后的图像转换，并将转换后的图像存储到缓存中，请参见 `SDImageTransformer`。
// 默认为 nil，这意味着不转换。
// 注意：如果你提供，这将影响 manager 的所有加载请求。但是，你可以通过上下文 `SDWebImageContextImageTransformer` 使用那个转换器来替换。
@property (strong, nonatomic, nullable) id<SDImageTransformer> transformer;

// The cache filter is used to convert an URL into a cache key each time SDWebImageManager need cache key to use image cache.
// The following example sets a filter in the application delegate that will remove any query-string from the URL before to use it as a cache key:
// 每次 SDWebImageManager 需要缓存密钥以使用图像缓存时，缓存过滤器用于将 URL 转换为缓存密钥。
// 以下示例在应用程序代理中设置一个过滤器，该过滤器将从 URL 中删除任何查询字符串，然后将其用作缓存键：

/**
 @code
 SDWebImageManager.sharedManager.cacheKeyFilter =[SDWebImageCacheKeyFilter cacheKeyFilterWithBlock:^NSString * _Nullable(NSURL * _Nonnull url) {
    url = [[NSURL alloc] initWithScheme:url.scheme host:url.host path:url.path];
    return [url absoluteString];
 }];
 @endcode
 */
@property (nonatomic, strong, nullable) id<SDWebImageCacheKeyFilter> cacheKeyFilter;

// The cache serializer is used to convert the decoded image, the source downloaded data, to the actual data used for storing to the disk cache. If you return nil, means to generate the data from the image instance, see `SDImageCache`.
// For example, if you are using WebP images and facing the slow decoding time issue when later retriving from disk cache again. You can try to encode the decoded image to JPEG/PNG format to disk cache instead of source downloaded data.
// @note The `image` arg is nonnull, but when you also provide a image transformer and the image is transformed, the `data` arg may be nil, take attention to this case.
// @note This method is called from a global queue in order to not to block the main thread.
// 缓存序列化器用于将解码后的图像（下载的源数据）转换为用于存储到磁盘缓存的实际数据。如果返回 nil，则表示要从图像实例生成数据，请参见 `SDImageCache`。
// 例如，如果你使用的是 WebP 图像，并且以后再次从磁盘缓存中检索时，会遇到解码时间慢的问题。您可以尝试将解码后的图像编码为 JPEG/PNG 格式到磁盘缓存，而不是源下载的数据。
// 注意：`image` 参数不为空，但是你还提供图像转换器并对图像进行转换时，`data` 参数可能为零，请注意这种情况。
// 注意：为了不阻塞主线程，从全局队列调用此方法。

/**
 @code
 SDWebImageManager.sharedManager.cacheSerializer = [SDWebImageCacheSerializer cacheSerializerWithBlock:^NSData * _Nullable(UIImage * _Nonnull image, NSData * _Nullable data, NSURL * _Nullable imageURL) {
    SDImageFormat format = [NSData sd_imageFormatForImageData:data];
    switch (format) {
        case SDImageFormatWebP:
            return image.images ? data : nil;
        default:
            return data;
    }
 }];
 @endcode
 */

// The default value is nil. Means we just store the source downloaded data to disk cache.
// 默认值为零。意味着我们只是将源下载的数据存储到磁盘缓存中。
@property (nonatomic, strong, nullable) id<SDWebImageCacheSerializer> cacheSerializer;

// The options processor is used, to have a global control for all the image request options and context option for current manager.
// @note If you use `transformer`, `cacheKeyFilter` or `cacheSerializer` property of manager, the input context option already apply those properties before passed. This options processor is a better replacement for those property in common usage.
// For example, you can control the global options, based on the URL or original context option like the below code.
// 选项处理器用于全局控制当前管理器的所有图像请求选项和上下文选项。
// 注意：如果使用管理器的 `transformer`、`cacheKeyFilter` 或 `cacheSerializer` 属性，则输入上下文选项在传递之前已经应用了这些属性。这个选项处理器可以更好地替换那些常用的属性。
// 例如，可以根据 URL 或原始上下文选项控制全局选项，如以下示例：

/**
 @code
 SDWebImageManager.sharedManager.optionsProcessor = [SDWebImageOptionsProcessor optionsProcessorWithBlock:^SDWebImageOptionsResult * _Nullable(NSURL * _Nullable url, SDWebImageOptions options, SDWebImageContext * _Nullable context) {
     // Only do animation on `SDAnimatedImageView`
     if (!context[SDWebImageContextAnimatedImageClass]) {
        options |= SDWebImageDecodeFirstFrameOnly;
     }
     // Do not force decode for png url
     if ([url.lastPathComponent isEqualToString:@"png"]) {
        options |= SDWebImageAvoidDecodeImage;
     }
     // Always use screen scale factor
     SDWebImageMutableContext *mutableContext = [NSDictionary dictionaryWithDictionary:context];
     mutableContext[SDWebImageContextImageScaleFactor] = @(UIScreen.mainScreen.scale);
     context = [mutableContext copy];
 
     return [[SDWebImageOptionsResult alloc] initWithOptions:options context:context];
 }];
 @endcode
 */
@property (nonatomic, strong, nullable) id<SDWebImageOptionsProcessor> optionsProcessor;

/**
 * Check one or more operations running
 */
@property (nonatomic, assign, readonly, getter=isRunning) BOOL running;

/**
 The default image cache when the manager which is created with no arguments. Such as shared manager or init.
 Defaults to nil. Means using `SDImageCache.sharedImageCache`
 */
@property (nonatomic, class, nullable) id<SDImageCache> defaultImageCache;

/**
 The default image loader for manager which is created with no arguments. Such as shared manager or init.
 Defaults to nil. Means using `SDWebImageDownloader.sharedDownloader`
 */
@property (nonatomic, class, nullable) id<SDImageLoader> defaultImageLoader;

/**
 * Returns global shared manager instance.
 */
@property (nonatomic, class, readonly, nonnull) SDWebImageManager *sharedManager;

/**
 * Allows to specify instance of cache and image loader used with image manager.
 * @return new instance of `SDWebImageManager` with specified cache and loader.
 */
- (nonnull instancetype)initWithCache:(nonnull id<SDImageCache>)cache loader:(nonnull id<SDImageLoader>)loader NS_DESIGNATED_INITIALIZER;

/**
 * Downloads the image at the given URL if not present in cache or return the cached version otherwise.
 *
 * @param url            The URL to the image
 * @param options        A mask to specify options to use for this request
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed.
 *
 *   This parameter is required.
 * 
 *   This block has no return value and takes the requested UIImage as first parameter and the NSData representation as second parameter.
 *   In case of error the image parameter is nil and the third parameter may contain an NSError.
 *
 *   The forth parameter is an `SDImageCacheType` enum indicating if the image was retrieved from the local cache
 *   or from the memory cache or from the network.
 *
 *   The fith parameter is set to NO when the SDWebImageProgressiveLoad option is used and the image is
 *   downloading. This block is thus called repeatedly with a partial image. When image is fully downloaded, the
 *   block is called a last time with the full image and the last parameter set to YES.
 *
 *   The last parameter is the original image URL
 *
 * @return Returns an instance of SDWebImageCombinedOperation, which you can cancel the loading process.
 */
- (nullable SDWebImageCombinedOperation *)loadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageOptions)options
                                                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                                 completed:(nonnull SDInternalCompletionBlock)completedBlock;

/**
 * Downloads the image at the given URL if not present in cache or return the cached version otherwise.
 *
 * @param url            The URL to the image
 * @param options        A mask to specify options to use for this request
 * @param context        A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param progressBlock  A block called while image is downloading
 *                       @note the progress block is executed on a background queue
 * @param completedBlock A block called when operation has been completed.
 *
 * @return Returns an instance of SDWebImageCombinedOperation, which you can cancel the loading process.
 */
- (nullable SDWebImageCombinedOperation *)loadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageOptions)options
                                                   context:(nullable SDWebImageContext *)context
                                                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                                 completed:(nonnull SDInternalCompletionBlock)completedBlock;

/**
 * Cancel all current operations
 */
- (void)cancelAll;

/**
 * Return the cache key for a given URL
 */
- (nullable NSString *)cacheKeyForURL:(nullable NSURL *)url;

@end
