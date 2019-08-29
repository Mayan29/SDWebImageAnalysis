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
#import "SDImageCacheConfig.h"
#import "SDImageCacheDefine.h"
#import "SDMemoryCache.h"
#import "SDDiskCache.h"

/// Image Cache Options
/// 图像缓存选项
typedef NS_OPTIONS(NSUInteger, SDImageCacheOptions) {
    // By default, we do not query image data when the image is already cached in memory. This mask can force to query image data at the same time. However, this query is asynchronously unless you specify `SDImageCacheQueryMemoryDataSync`
    // 默认情况下，当图像已缓存在内存中时，我们不会查询图像数据。此标识可以强制同时查询图像数据。但是，除非指定 `SDImageCacheQueryMemoryDataSync` 否则此查询是异步的
    SDImageCacheQueryMemoryData = 1 << 0,
    // By default, when you only specify `SDImageCacheQueryMemoryData`, we query the memory image data asynchronously. Combined this mask as well to query the memory image data synchronously.
    // 默认情况下，当你只指定 `SDImageCacheQueryMemoryData` 时，我们会异步查询内存图像数据。同时结合此标识来同步查询内存图像数据。
    SDImageCacheQueryMemoryDataSync = 1 << 1,
    // By default, when the memory cache miss, we query the disk cache asynchronously. This mask can force to query disk cache (when memory cache miss) synchronously.
    // @note These 3 query options can be combined together. For the full list about these masks combination, see wiki page.
    // 默认情况下，当内存缓存 miss 时，我们异步查询磁盘缓存。此标识可以强制同步查询磁盘缓存（当内存缓存 miss 时）。
    // 注意：这 3 个查询选项可以组合在一起。有关这些标识组合的完整列表，请参见 wiki 页面。
    SDImageCacheQueryDiskDataSync = 1 << 2,
    // By default, images are decoded respecting their original size. On iOS, this flag will scale down the images to a size compatible with the constrained memory of devices.
    // 默认情况下，图像会根据其原始大小进行解码。在 iOS 上，此标识会将图像缩小到与设备的受限内存兼容的大小。
    SDImageCacheScaleDownLargeImages = 1 << 3,
    // By default, we will decode the image in the background during cache query and download from the network. This can help to improve performance because when rendering image on the screen, it need to be firstly decoded. But this happen on the main queue by Core Animation.
    // However, this process may increase the memory usage as well. If you are experiencing a issue due to excessive memory consumption, This flag can prevent decode the image.
    // 默认情况下，我们将在缓存查询期间在后台解码图像并从网络下载。这有助于提高性能，因为在屏幕上渲染图像时，需要首先对其进行解码。但这发生在 Core Animation 的主队列中。
    // 但是，此过程也可能会增加内存使用量。如果由于过多的内存消耗而遇到问题，此标识可以防止解码图像。
    SDImageCacheAvoidDecodeImage = 1 << 4,
    // By default, we decode the animated image. This flag can force decode the first frame only and produece the static image.
    // 默认情况下，我们解码动画图像。此标志只能强制解码第一帧并生成静态图像。
    SDImageCacheDecodeFirstFrameOnly = 1 << 5,
    // By default, for `SDAnimatedImage`, we decode the animated image frame during rendering to reduce memory usage. This flag actually trigger `preloadAllAnimatedImageFrames = YES` after image load from disk cache
    // 默认情况下，对于 `SDAnimatedImage`，我们在渲染过程中解码动画图像帧，以减少内存使用。从磁盘缓存加载图像后，此标志实际会触发 `preloadAllAnimatedImageFrames = YES`
    SDImageCachePreloadAllFrames = 1 << 6,
    // By default, when you use `SDWebImageContextAnimatedImageClass` context option (like using `SDAnimatedImageView` which designed to use `SDAnimatedImage`), we may still use `UIImage` when the memory cache hit, or image decoder is not available, to behave as a fallback solution.
    // Using this option, can ensure we always produce image with your provided class. If failed, a error with code `SDWebImageErrorBadImageData` will been used.
    // Note this options is not compatible with `SDImageCacheDecodeFirstFrameOnly`, which always produce a UIImage/NSImage.
    // 默认情况下，当你使用 `SDWebImageContextAnimatedImageClass` 上下文选项时（比如使用 `SDAnimatedImageView` 设计使用 `SDAnimatedImage`），当内存缓存或图像解码器不可用时，我们仍然可以使用 `UIImage` 作为后备解决方案。
    // 使用此选项，可以确保我们始终使用你提供的 class 生成图像。如果失败，将使用代码为 `SDWebImageErrorBadImageData` 的错误。
    // 注意这个选项与 `SDImageCacheDecodeFirstFrameOnly` 不兼容，后者始终生成 UIImage/NSImage。
    SDImageCacheMatchAnimatedImageClass = 1 << 7,
};

// SDImageCache maintains a memory cache and a disk cache. Disk cache write operations are performed asynchronous so it doesn’t add unnecessary latency to the UI.
// SDImageCache 维护一个内存缓存和一个磁盘缓存。磁盘缓存写入操作是异步执行的，因此不会给 UI 增加不必要的延迟。
@interface SDImageCache : NSObject

#pragma mark - Properties

/**
 *  Cache Config object - storing all kind of settings.
 *  The property is copy so change of currrent config will not accidentally affect other cache's config.
 */
@property (nonatomic, copy, nonnull, readonly) SDImageCacheConfig *config;

// The memory cache implementation object used for current image cache.
// By default we use `SDMemoryCache` class, you can also use this to call your own implementation class method.
// @note To customize this class, check `SDImageCacheConfig.memoryCacheClass` property.
// 用于当前图像缓存的内存缓存实现对象。
// 默认情况下，我们使用 `SDMemoryCache` 类，您也可以使用它调用自己的实现类方法。
// 注意：若要自定义此类，请选择 `SDImageCacheConfig.memoryCacheClass` 属性。
@property (nonatomic, strong, readonly, nonnull) id<SDMemoryCache> memoryCache;

// The disk cache implementation object used for current image cache.
// By default we use `SDMemoryCache` class, you can also use this to call your own implementation class method.
// @note To customize this class, check `SDImageCacheConfig.diskCacheClass` property.
// @warning When calling method about read/write in disk cache, be sure to either make your disk cache implementation IO-safe or using the same access queue to avoid issues.
// 用于当前图像缓存的磁盘缓存实现对象。
// 默认情况下，我们使用 `SDMemoryCache` 类，您也可以使用它调用自己的实现类方法。
// 注意：若要自定义此类，请选择 `SDImageCacheConfig.diskCacheClass` 属性。
// 警告：调用磁盘缓存中的 read/write 方法时，请确保磁盘缓存实现 IO-safe，或使用相同的访问队列以避免出现问题。
@property (nonatomic, strong, readonly, nonnull) id<SDDiskCache> diskCache;

/**
 *  The disk cache's root path
 */
@property (nonatomic, copy, nonnull, readonly) NSString *diskCachePath;

/**
 *  The additional disk cache path to check if the query from disk cache not exist;
 *  The `key` param is the image cache key. The returned file path will be used to load the disk cache. If return nil, ignore it.
 *  Useful if you want to bundle pre-loaded images with your app
 */
@property (nonatomic, copy, nullable) SDImageCacheAdditionalCachePathBlock additionalCachePathBlock;

#pragma mark - Singleton and initialization

// Returns global shared cache instance
// 返回全局共享缓存实例
@property (nonatomic, class, readonly, nonnull) SDImageCache *sharedImageCache;

/**
 * Init a new cache store with a specific namespace
 *
 * @param ns The namespace to use for this cache store
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns;

/**
 * Init a new cache store with a specific namespace and directory.
 * If you don't provide the disk cache directory, we will use the User Cache directory with prefix (~/Library/Caches/com.hackemist.SDImageCache/).
 *
 * @param ns        The namespace to use for this cache store
 * @param directory Directory to cache disk images in
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns
                       diskCacheDirectory:(nullable NSString *)directory;

/**
 * Init a new cache store with a specific namespace, directory and file manager
 * The final disk cache directory should looks like ($directory/$namespace). And the default config of shared cache, should result in (~/Library/Caches/com.hackemist.SDImageCache/default/)
 *
 * @param ns          The namespace to use for this cache store
 * @param directory   Directory to cache disk images in
 * @param config      The cache config to be used to create the cache. You can provide custom memory cache or disk cache class in the cache config
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns
                       diskCacheDirectory:(nullable NSString *)directory
                                   config:(nullable SDImageCacheConfig *)config NS_DESIGNATED_INITIALIZER;

#pragma mark - Cache paths

/**
 Get the cache path for a certain key
 
 @param key The unique image cache key
 @return The cache path. You can check `lastPathComponent` to grab the file name.
 */
- (nullable NSString *)cachePathForKey:(nullable NSString *)key;

#pragma mark - Store Ops

/**
 * Asynchronously store an image into memory and disk cache at the given key.
 *
 * @param image           The image to store
 * @param key             The unique image cache key, usually it's image absolute URL
 * @param completionBlock A block executed after the operation is finished
 */
- (void)storeImage:(nullable UIImage *)image
            forKey:(nullable NSString *)key
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * Asynchronously store an image into memory and disk cache at the given key.
 *
 * @param image           The image to store
 * @param key             The unique image cache key, usually it's image absolute URL
 * @param toDisk          Store the image to disk cache if YES. If NO, the completion block is called synchronously
 * @param completionBlock A block executed after the operation is finished
 */
- (void)storeImage:(nullable UIImage *)image
            forKey:(nullable NSString *)key
            toDisk:(BOOL)toDisk
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * Asynchronously store an image into memory and disk cache at the given key.
 *
 * @param image           The image to store
 * @param imageData       The image data as returned by the server, this representation will be used for disk storage
 *                        instead of converting the given image object into a storable/compressed image format in order
 *                        to save quality and CPU
 * @param key             The unique image cache key, usually it's image absolute URL
 * @param toDisk          Store the image to disk cache if YES. If NO, the completion block is called synchronously
 * @param completionBlock A block executed after the operation is finished
 */
- (void)storeImage:(nullable UIImage *)image
         imageData:(nullable NSData *)imageData
            forKey:(nullable NSString *)key
            toDisk:(BOOL)toDisk
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * Synchronously store image into memory cache at the given key.
 *
 * @param image  The image to store
 * @param key    The unique image cache key, usually it's image absolute URL
 */
- (void)storeImageToMemory:(nullable UIImage*)image
                    forKey:(nullable NSString *)key;

/**
 * Synchronously store image data into disk cache at the given key.
 *
 * @param imageData  The image data to store
 * @param key        The unique image cache key, usually it's image absolute URL
 */
- (void)storeImageDataToDisk:(nullable NSData *)imageData
                      forKey:(nullable NSString *)key;


#pragma mark - Contains and Check Ops

/**
 *  Asynchronously check if image exists in disk cache already (does not load the image)
 *
 *  @param key             the key describing the url
 *  @param completionBlock the block to be executed when the check is done.
 *  @note the completion block will be always executed on the main queue
 */
- (void)diskImageExistsWithKey:(nullable NSString *)key completion:(nullable SDImageCacheCheckCompletionBlock)completionBlock;

/**
 *  Synchronously check if image data exists in disk cache already (does not load the image)
 *
 *  @param key             the key describing the url
 */
- (BOOL)diskImageDataExistsWithKey:(nullable NSString *)key;

#pragma mark - Query and Retrieve Ops

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 *  Query the image data for the given key synchronously.
 *
 *  @param key The unique key used to store the wanted image
 *  @return The image data for the given key, or nil if not found.
 */
- (nullable NSData *)diskImageDataForKey:(nullable NSString *)key;

/**
 * Operation that queries the cache asynchronously and call the completion when done.
 *
 * @param key       The unique key used to store the wanted image
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 *
 * @return a NSOperation instance containing the cache op
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 *
 * @param key       The unique key used to store the wanted image
 * @param options   A mask to specify options to use for this cache query
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 *
 * @return a NSOperation instance containing the cache op
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key options:(SDImageCacheOptions)options done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Asynchronously queries the cache with operation and call the completion when done.
 *
 * @param key       The unique key used to store the wanted image
 * @param options   A mask to specify options to use for this cache query
 * @param context   A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
 * @param doneBlock The completion block. Will not get called if the operation is cancelled
 *
 * @return a NSOperation instance containing the cache op
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key options:(SDImageCacheOptions)options context:(nullable SDWebImageContext *)context done:(nullable SDImageCacheQueryCompletionBlock)doneBlock;

/**
 * Synchronously query the memory cache.
 *
 * @param key The unique key used to store the image
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromMemoryCacheForKey:(nullable NSString *)key;

/**
 * Synchronously query the disk cache.
 *
 * @param key The unique key used to store the image
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromDiskCacheForKey:(nullable NSString *)key;

/**
 * Synchronously query the cache (memory and or disk) after checking the memory cache.
 *
 * @param key The unique key used to store the image
 * @return The image for the given key, or nil if not found.
 */
- (nullable UIImage *)imageFromCacheForKey:(nullable NSString *)key;

#pragma mark - Remove Ops

/**
 * Asynchronously remove the image from memory and disk cache
 *
 * @param key             The unique image cache key
 * @param completion      A block that should be executed after the image has been removed (optional)
 */
- (void)removeImageForKey:(nullable NSString *)key withCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 * Asynchronously remove the image from memory and optionally disk cache
 *
 * @param key             The unique image cache key
 * @param fromDisk        Also remove cache entry from disk if YES. If NO, the completion block is called synchronously
 * @param completion      A block that should be executed after the image has been removed (optional)
 */
- (void)removeImageForKey:(nullable NSString *)key fromDisk:(BOOL)fromDisk withCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 Synchronously remove the image from memory cache.
 
 @param key The unique image cache key
 */
- (void)removeImageFromMemoryForKey:(nullable NSString *)key;

/**
 Synchronously remove the image from disk cache.
 
 @param key The unique image cache key
 */
- (void)removeImageFromDiskForKey:(nullable NSString *)key;

#pragma mark - Cache clean Ops

/**
 * Synchronously Clear all memory cached images
 */
- (void)clearMemory;

/**
 * Asynchronously clear all disk cached images. Non-blocking method - returns immediately.
 * @param completion    A block that should be executed after cache expiration completes (optional)
 */
- (void)clearDiskOnCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 * Asynchronously remove all expired cached image from disk. Non-blocking method - returns immediately.
 * @param completionBlock A block that should be executed after cache expiration completes (optional)
 */
- (void)deleteOldFilesWithCompletionBlock:(nullable SDWebImageNoParamsBlock)completionBlock;

#pragma mark - Cache Info

/**
 * Get the total bytes size of images in the disk cache
 */
- (NSUInteger)totalDiskSize;

/**
 * Get the number of images in the disk cache
 */
- (NSUInteger)totalDiskCount;

/**
 * Asynchronously calculate the disk cache's size.
 */
- (void)calculateSizeWithCompletionBlock:(nullable SDImageCacheCalculateSizeBlock)completionBlock;

@end

// SDImageCache is the built-in image cache implementation for web image manager. It adopts `SDImageCache` protocol to provide the function for web image manager to use for image loading process.
// SDImageCache 是 Web 图像管理器的内置图像缓存实现。它采用 `SDImageCache` 协议为 Web 图像管理器提供用于图像加载过程的功能。
@interface SDImageCache (SDImageCache) <SDImageCache>

@end
