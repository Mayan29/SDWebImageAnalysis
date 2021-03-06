/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"
#import "SDWebImageDefine.h"

/// Image Cache Type
/// 图像存储类型
typedef NS_ENUM(NSInteger, SDImageCacheType) {
    
    // For query and contains op in response, means the image isn't available in the image cache
    // For op in request, this type is not available and take no effect.
    // 对于查询和响应，表示图像在图像缓存中不可用。
    // 对于请求，表示不可用且不起作用。
    SDImageCacheTypeNone,

    // For query and contains op in response, means the image was obtained from the disk cache.
    // For op in request, means process only disk cache.
    // 对于查询和响应，表示图像是从磁盘缓存中获取的。
    // 对于请求，表示只处理磁盘缓存。
    SDImageCacheTypeDisk,

    // For query and contains op in response, means the image was obtained from the memory cache.
    // For op in request, means process only memory cache.
    // 对于查询和响应，表示图像是从内存缓存中获取的。
    // 对于请求，表示只处理内存缓存。
    SDImageCacheTypeMemory,

    // For query and contains op in response, this type is not available and take no effect.
    // For op in request, means process both memory cache and disk cache.
    // 对于查询和响应，表示图像在图像缓存中不可用。
    // 对于请求，表示同时处理内存缓存和磁盘缓存。
    SDImageCacheTypeAll
};

typedef void(^SDImageCacheCheckCompletionBlock)(BOOL isInCache);
typedef void(^SDImageCacheCalculateSizeBlock)(NSUInteger fileCount, NSUInteger totalSize);
typedef NSString * _Nullable (^SDImageCacheAdditionalCachePathBlock)(NSString * _Nonnull key);
typedef void(^SDImageCacheQueryCompletionBlock)(UIImage * _Nullable image, NSData * _Nullable data, SDImageCacheType cacheType);
typedef void(^SDImageCacheContainsCompletionBlock)(SDImageCacheType containsCacheType);

/**
 This is the built-in decoding process for image query from cache.
 @note If you want to implement your custom loader with `queryImageForKey:options:context:completion:` API, but also want to keep compatible with SDWebImage's behavior, you'd better use this to produce image.
 
 @param imageData The image data from the cache. Should not be nil
 @param cacheKey The image cache key from the input. Should not be nil
 @param options The options arg from the input
 @param context The context arg from the input
 @return The decoded image for current image data query from cache
 */
FOUNDATION_EXPORT UIImage * _Nullable SDImageCacheDecodeImageData(NSData * _Nonnull imageData, NSString * _Nonnull cacheKey, SDWebImageOptions options, SDWebImageContext * _Nullable context);

// This is the image cache protocol to provide custom image cache for `SDWebImageManager`.
// Though the best practice to custom image cache, is to write your own class which conform `SDMemoryCache` or `SDDiskCache` protocol for `SDImageCache` class (See more on `SDImageCacheConfig.memoryCacheClass & SDImageCacheConfig.diskCacheClass`).
// However, if your own cache implementation contains more advanced feature beyond `SDImageCache` itself, you can consider to provide this instead. For example, you can even use a cache manager like `SDImageCachesManager` to register multiple caches.
// 这是为 `SDWebImageManager` 提供自定义图像缓存的图像缓存协议。
// 虽然自定义图像缓存的最佳方案是编写自己的类，该类符合 `SDMemoryCache` 类的 `SDMemoryCache` 或 `SDDiskCache` 协议（请参见 `SDImageCacheConfig.memoryCacheClass & SDImageCacheConfig.diskCacheClass` 上的详细信息）。
// 但是，如果你自己的缓存实现包含了比 `SDImageCache` 本身更高级的功能，则可以考虑提供此功能。例如，你甚至可以使用类似 `SDImageCachesManager` 的缓存管理器来注册多个缓存。
@protocol SDImageCache <NSObject>

@required
// Query the cached image from image cache for given key. The operation can be used to cancel the query.
// If image is cached in memory, completion is called synchronously, else aynchronously and depends on the options arg (See `SDWebImageQueryDiskSync`)
// 根据提供的 key 从图像缓存中查询缓存的图像。该操作可用于取消查询。
// 如果图像缓存在内存中，则会同步调用 completion，否则将异步调用，并依赖于选项值（请参见 `SDWebImageQueryDiskSync`）。
- (nullable id<SDWebImageOperation>)queryImageForKey:(nullable NSString *)key
                                             options:(SDWebImageOptions)options
                                             context:(nullable SDWebImageContext *)context
                                          completion:(nullable SDImageCacheQueryCompletionBlock)completionBlock;

/**
 Store the image into image cache for the given key. If cache type is memory only, completion is called synchronously, else aynchronously.

 @param image The image to store
 @param imageData The image data to be used for disk storage
 @param key The image cache key
 @param cacheType The image store op cache type
 @param completionBlock A block executed after the operation is finished
 */
- (void)storeImage:(nullable UIImage *)image
         imageData:(nullable NSData *)imageData
            forKey:(nullable NSString *)key
         cacheType:(SDImageCacheType)cacheType
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 Remove the image from image cache for the given key. If cache type is memory only, completion is called synchronously, else aynchronously.

 @param key The image cache key
 @param cacheType The image remove op cache type
 @param completionBlock A block executed after the operation is finished
 */
- (void)removeImageForKey:(nullable NSString *)key
                cacheType:(SDImageCacheType)cacheType
               completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 Check if image cache contains the image for the given key (does not load the image). If image is cached in memory, completion is called synchronously, else aynchronously.

 @param key The image cache key
 @param cacheType The image contains op cache type
 @param completionBlock A block executed after the operation is finished.
 */
- (void)containsImageForKey:(nullable NSString *)key
                  cacheType:(SDImageCacheType)cacheType
                 completion:(nullable SDImageCacheContainsCompletionBlock)completionBlock;

/**
 Clear all the cached images for image cache. If cache type is memory only, completion is called synchronously, else aynchronously.

 @param cacheType The image clear op cache type
 @param completionBlock A block executed after the operation is finished
 */
- (void)clearWithCacheType:(SDImageCacheType)cacheType
                completion:(nullable SDWebImageNoParamsBlock)completionBlock;

@end
