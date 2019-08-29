/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

@class SDImageCacheConfig;
// A protocol to allow custom memory cache used in SDImageCache.
// 允许在 SDImageCache 中使用自定义内存缓存的协议。
@protocol SDMemoryCache <NSObject>

@required
// Create a new memory cache instance with the specify cache config. You can check `maxMemoryCost` and `maxMemoryCount` used for memory cache.
// 使用指定缓存配置创建新的内存缓存实例。您可以检查用于内存缓存的 `maxMemoryCost` 和 `maxMemoryCount`。
- (nonnull instancetype)initWithConfig:(nonnull SDImageCacheConfig *)config;

// Returns the value associated with a given key.
// 返回与给定 key 关联的 value。
- (nullable id)objectForKey:(nonnull id)key;

/**
 Sets the value of the specified key in the cache (0 cost).
 
 @param object The object to be stored in the cache. If nil, it calls `removeObjectForKey:`.
 @param key    The key with which to associate the value. If nil, this method has no effect.
 @discussion Unlike an NSMutableDictionary object, a cache does not copy the key
 objects that are put into it.
 */
- (void)setObject:(nullable id)object forKey:(nonnull id)key;

/**
 Sets the value of the specified key in the cache, and associates the key-value
 pair with the specified cost.
 
 @param object The object to store in the cache. If nil, it calls `removeObjectForKey`.
 @param key    The key with which to associate the value. If nil, this method has no effect.
 @param cost   The cost with which to associate the key-value pair.
 @discussion Unlike an NSMutableDictionary object, a cache does not copy the key
 objects that are put into it.
 */
- (void)setObject:(nullable id)object forKey:(nonnull id)key cost:(NSUInteger)cost;

/**
 Removes the value of the specified key in the cache.
 
 @param key The key identifying the value to be removed. If nil, this method has no effect.
 */
- (void)removeObjectForKey:(nonnull id)key;

/**
 Empties the cache immediately.
 */
- (void)removeAllObjects;

@end

// A memory cache which auto purge the cache on memory warning and support weak cache.
// 一种内存缓存，在内存出现警告时自动清除缓存并支持 weak 缓存。
@interface SDMemoryCache <KeyType, ObjectType> : NSCache <KeyType, ObjectType> <SDMemoryCache>

@property (nonatomic, strong, nonnull, readonly) SDImageCacheConfig *config;

@end
