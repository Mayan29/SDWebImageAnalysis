/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// Image Cache Expire Type
/// 图像缓存过期类型
typedef NS_ENUM(NSUInteger, SDImageCacheConfigExpireType) {
    
    // When the image is accessed it will update this value
    // 图像最近访问的时间
    SDImageCacheConfigExpireTypeAccessDate,
    // The image was obtained from the disk cache (Default)
    // 图像最近修改的时间（默认）
    SDImageCacheConfigExpireTypeModificationDate
};

// The class contains all the config for image cache
// @note This class conform to NSCopying, make sure to add the property in `copyWithZone:` as well.
// 该类包含图像缓存的所有配置
// 注意：这个 class 遵守 NSCopying，确保在 `copyWithZone:` 中添加属性。
@interface SDImageCacheConfig : NSObject <NSCopying>

// Gets the default cache config used for shared instance or initialization when it does not provide any cache config. Such as `SDImageCache.sharedImageCache`.
// @note You can modify the property on default cache config, which can be used for later created cache instance. The already created cache instance does not get affected.
// 获取默认缓存配置，在 shared instance 或初始化不提供任何缓存配置时。例如 `SDImageCache.sharedImageCache`。
// 注意：可以修改默认缓存配置的属性，该属性可用于以后创建的缓存实例。已创建的缓存实例不会受到影响。
@property (nonatomic, class, readonly, nonnull) SDImageCacheConfig *defaultCacheConfig;

// Whether or not to disable iCloud backup
// Defaults to YES.
// 是否禁用 iCloud 备份。
// 默认为 YES。
@property (assign, nonatomic) BOOL shouldDisableiCloud;

// Whether or not to use memory cache
// @note When the memory cache is disabled, the weak memory cache will also be disabled.
// Defaults to YES.
// 是否使用内存缓存。
// 注意：禁用内存缓存时，也会禁用弱内存缓存。
// 默认为 YES。
@property (assign, nonatomic) BOOL shouldCacheImagesInMemory;

// The option to control weak memory cache for images. When enable, `SDImageCache`'s memory cache will use a weak maptable to store the image at the same time when it stored to memory, and get removed at the same time.
// However when memory warning is triggered, since the weak maptable does not hold a strong reference to image instance, even when the memory cache itself is purged, some images which are held strongly by UIImageViews or other live instances can be recovered again, to avoid later re-query from disk cache or network. This may be helpful for the case, for example, when app enter background and memory is purged, cause cell flashing after re-enter foreground.
// Defautls to YES. You can change this option dynamically.
// 控制图像的弱内存缓存。启用时，`SDImageCache` 的内存缓存将使用弱映射表在存储到内存的同时存储图像，并同时删除。
// 但是当触发内存警告时，由于弱映射表没有对图像实例的强引用，即使清除了内存缓存本身，也可以再次恢复 UIImageViews 或其他实例保留的一些图像，避免之后从磁盘缓存或网络重新查询。这可能对这种情况有所帮助，例如，当 app 进入后台并清除内存时，会在重新进入前台后导致 cell 闪烁。
// 默认为 YES。你可以动态更改此选项。
@property (assign, nonatomic) BOOL shouldUseWeakMemoryCache;

// Whether or not to remove the expired disk data when application entering the background. (Not works for macOS)
// Defaults to YES.
// 应用程序进入后台时是否删除过期的磁盘数据。（不适用于 MacOS）
// 默认为 YES。
@property (assign, nonatomic) BOOL shouldRemoveExpiredDataWhenEnterBackground;

// The reading options while reading cache from disk.
// Defaults to 0. You can set this to `NSDataReadingMappedIfSafe` to improve performance.
// 从磁盘读取缓存时的读取选项。
// 默认为 0。你可以将其设置为 `NSDataReadingMappedIfSafe` 以提高性能。
@property (assign, nonatomic) NSDataReadingOptions diskCacheReadingOptions;

// The writing options while writing cache to disk.
// Defaults to `NSDataWritingAtomic`. You can set this to `NSDataWritingWithoutOverwriting` to prevent overwriting an existing file.
// 将缓存写入磁盘时的写入选项。
// 默认为 `NSDataWritingAtomic`。你可以将其设置为 `NSDataWritingWithoutOverwriting` 以防止覆盖现有文件。
@property (assign, nonatomic) NSDataWritingOptions diskCacheWritingOptions;

// The maximum length of time to keep an image in the disk cache, in seconds.
// Setting this to a negative value means no expiring.
// Setting this to zero means that all cached files would be removed when do expiration check.
// Defaults to 1 week.
// 在磁盘缓存中保存图像的最长时间（以秒为单位）。
// 设置为负值表示不会过期。
// 设置为 0 意味着在执行过期检查时，删除所有缓存的文件。
// 默认为 1 周。
@property (assign, nonatomic) NSTimeInterval maxDiskAge;

// The maximum size of the disk cache, in bytes.
// Defaults to 0. Which means there is no cache size limit.
// 磁盘缓存的最大 size（以字节为单位）。
// 默认为 0。这意味着没有缓存大小限制。
@property (assign, nonatomic) NSUInteger maxDiskSize;

// The maximum "total cost" of the in-memory image cache. The cost function is the bytes size held in memory.
// @note The memory cost is bytes size in memory, but not simple pixels count. For common ARGB8888 image, one pixel is 4 bytes (32 bits).
// Defaults to 0. Which means there is no memory cost limit.
// 内存中图像缓存的最大「总成本」。成本作用是内存中保存的字节大小。
// 注意：内存成本是内存中的字节大小，但不是简单的像素计数。对于普通 ARGB8888 图像，一个像素是 4 字节（32 位）。
// 默认为 0。这意味着没有内存成本限制。
@property (assign, nonatomic) NSUInteger maxMemoryCost;

// The maximum number of objects in-memory image cache should hold.
// Defaults to 0. Which means there is no memory count limit.
// 应该保留内存中图像缓存中的对象最大数量。
// 默认为0。这意味着没有内存数量限制。
@property (assign, nonatomic) NSUInteger maxMemoryCount;

// The attribute which the clear cache will be checked against when clearing the disk cache
// Default is Modified Date
// 清除磁盘缓存时将检查清除缓存的属性
// 默认为修改日期
@property (assign, nonatomic) SDImageCacheConfigExpireType diskCacheExpireType;

// The custom file manager for disk cache. Pass nil to let disk cache choose the proper file manager.
// Defaults to nil.
// @note This value does not support dynamic changes. Which means further modification on this value after cache initlized has no effect.
// @note Since `NSFileManager` does not support `NSCopying`. We just pass this by reference during copying. So it's not recommend to set this value on `defaultCacheConfig`.
// 磁盘缓存的自定义文件管理器。传递 nil 以让磁盘缓存选择正确的文件管理器。
// 默认为 nil。
// 注意：此值不支持动态更改。这意味着缓存初始化后对该值的进一步修改没有效果。
// 注意：`NSFileManager` 不支持 `NSCopying`。我们只是在复制过程中通过引用。因此，建议不要在 `defaultCacheConfig` 上设置此值。
@property (strong, nonatomic, nullable) NSFileManager *fileManager;

// The custom memory cache class. Provided class instance must conform to `SDMemoryCache` protocol to allow usage.
// Defaults to built-in `SDMemoryCache` class.
// @note This value does not support dynamic changes. Which means further modification on this value after cache initlized has no effect.
// 自定义内存缓存类。提供的类实例必须符合 `SDMemoryCache` 协议才能使用。
// 默认为内置的 `SDMemoryCache` 类。
// 注意：此值不支持动态更改。这意味着缓存初始化后对该值的进一步修改没有效果。
@property (assign, nonatomic, nonnull) Class memoryCacheClass;

// The custom disk cache class. Provided class instance must conform to `SDDiskCache` protocol to allow usage.
// Defaults to built-in `SDDiskCache` class.
// @note This value does not support dynamic changes. Which means further modification on this value after cache initlized has no effect.
// 自定义磁盘缓存类。提供的类实例必须符合 `SDDiskCache` 协议才能允许使用。
// 默认为内置 `SDDiskCache` 类。
// 注意：此值不支持动态更改。这意味着缓存初始化后对该值的进一步修改没有效果。
@property (assign ,nonatomic, nonnull) Class diskCacheClass;

@end
