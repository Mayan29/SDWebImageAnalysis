/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

// UIImage category for memory cache cost.
// 用于内存缓存开销的 UIImage 类别
@interface UIImage (MemoryCacheCost)

// The memory cache cost for specify image used by image cache. The cost function is the bytes size held in memory.
// If you set some associated object to `UIImage`, you can set the custom value to indicate the memory cost.
// 指定由图像缓存使用的图像的内存缓存开销。cost 函数是内存中保存的字节大小。
// 如果将某个关联对象设置为 `UIImage`，则可以设置自定义值以指示内存开销。
 
// For `UIImage`, this method return the single frame bytes size when `image.images` is nil for static image. Retuen full frame bytes size when `image.images` is not nil for animated image.
// For `NSImage`, this method return the single frame bytes size because `NSImage` does not store all frames in memory.
// @note Note that because of the limitations of category this property can get out of sync if you create another instance with CGImage or other methods.
// @note For custom animated class conforms to `SDAnimatedImage`, you can override this getter method in your subclass to return a more proper value instead, which representing the current frame's total bytes.
// 对于 `UIImage`，当静态图像的 `image.images` 为 nil 时，此方法返回单帧字节大小。当 `image.images` 不为 nil 时，返回全帧字节大小。
// 对于 `NSImage`，此方法返回单帧字节大小，因为 `NSImage` 不将所有帧存储在内存中。
// 注意：由于类别的限制，如果使用 CGImage 或其他方法创建另一个实例，则此属性可能会失去同步。
// 注意：对于符合 `SDAnimatedImage` 的自定义动画类，可以重写子类中的 getter 方法，以返回更合适的值，该值表示当前帧的总字节数。
@property (assign, nonatomic) NSUInteger sd_memoryCost;

@end
