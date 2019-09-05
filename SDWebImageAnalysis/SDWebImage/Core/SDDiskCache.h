/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

@class SDImageCacheConfig;

// A protocol to allow custom disk cache used in SDImageCache.
// 允许在 SDImageCache 中使用自定义磁盘缓存的协议。
@protocol SDDiskCache <NSObject>

// All of these method are called from the same global queue to avoid blocking on main queue and thread-safe problem. But it's also recommend to ensure thread-safe yourself using lock or other ways.
// 所有这些方法都从同一个全局队列调用，以避免阻塞主队列和线程安全问题。但是也建议使用锁或其他方法来确保自己的线程安全。
@required

// Create a new disk cache based on the specified path. You can check `maxDiskSize` and `maxDiskAge` used for disk cache.
// cachePath: Full path of a directory in which the cache will write data. Once initialized you should not read and write to this directory.
// 根据指定的路径创建新的磁盘缓存。您可以检查用于磁盘缓存的 `maxDiskSize` 和 `maxDiskAge`。
// cachePath: 缓存将写入数据的目录的全路径。初始化后你不应该读写此目录。
- (nullable instancetype)initWithCachePath:(nonnull NSString *)cachePath config:(nonnull SDImageCacheConfig *)config;

// Returns a boolean value that indicates whether a given key is in cache.
// This method may blocks the calling thread until file read finished.
// 根据 key 判断 value 是否存在。
// 此方法可能会阻止调用线程，直到文件读取完成。
- (BOOL)containsDataForKey:(nonnull NSString *)key;

// Returns the data associated with a given key.
// This method may blocks the calling thread until file read finished.
// 根据 key 获取 value。
// 此方法可能会阻塞调用线程，直到文件读取完成。
- (nullable NSData *)dataForKey:(nonnull NSString *)key;

// Sets the value of the specified key in the cache.
// This method may blocks the calling thread until file write finished.
// 根据 key 缓存 value。
// 此方法可能会阻塞调用线程，直到文件写入完成。
- (void)setData:(nullable NSData *)data forKey:(nonnull NSString *)key;

// Removes the value of the specified key in the cache.
// This method may blocks the calling thread until file delete finished.
// 根据 key 删除缓存中的 value。
// 此方法可能会阻塞调用线程，直到文件删除完成。
- (void)removeDataForKey:(nonnull NSString *)key;

// Empties the cache.
// This method may blocks the calling thread until file delete finished.
// 清空缓存。
// 此方法可能会阻塞调用线程，直到文件删除完成。
- (void)removeAllData;

// Removes the expired data from the cache. You can choose the data to remove base on `ageLimit`, `countLimit` and `sizeLimit` options.
// 从缓存中删除过期数据。可以根据 `ageLimit`，`countLimit` 和 `sizeLimit` 选项选择要删除的数据。
- (void)removeExpiredData;

// The cache path for key
// 根据 key 获取缓存路径
- (nullable NSString *)cachePathForKey:(nonnull NSString *)key;

// Returns the number of data in this cache.
// This method may blocks the calling thread until file read finished.
// 返回总数据计数。
// 此方法可能会阻塞调用线程，直到文件读取完成。
- (NSUInteger)totalCount;

// Returns the total size (in bytes) of data in this cache.
// This method may blocks the calling thread until file read finished.
// 返回总数据大小（字节）
// 此方法可能会阻塞调用线程，直到文件读取完成。
- (NSUInteger)totalSize;

@end

// The built-in disk cache.
// 内置磁盘缓存
@interface SDDiskCache : NSObject <SDDiskCache>

// Cache Config object - storing all kind of settings.
// 缓存配置对象 - 存储所有类型的设置
@property (nonatomic, strong, readonly, nonnull) SDImageCacheConfig *config;

- (nonnull instancetype)init NS_UNAVAILABLE;

// Move the cache directory from old location to new location, the old location will be removed after finish.
// If the old location does not exist, does nothing.
// If the new location does not exist, only do a movement of directory.
// If the new location does exist, will move and merge the files from old location.
// If the new location does exist, but is not a directory, will remove it and do a movement of directory.
// 将缓存目录从 old location 移动到 new location，完成后将删除 old location。
// 如果 old location 不存在，则不执行任何操作。
// 如果 new location 不存在，只移动目录。
// 如果 new location 存在，将移动并合并 old location 的文件。
// 如果 new location 确实存在，但不是目录，则会将其删除并移动目录。
- (void)moveCacheDirectoryFromPath:(nonnull NSString *)srcPath toPath:(nonnull NSString *)dstPath;

@end
