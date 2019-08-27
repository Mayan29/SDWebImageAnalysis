/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"

// These methods are used to support canceling for UIView image loading, it's designed to be used internal but not external.
// All the stored operations are weak, so it will be dalloced after image loading finished. If you need to store operations, use your own class to keep a strong reference for them.
// 这些方法用于支持取消 UIView 图像加载，它被设计为内部而非外部使用。
// 所有存储操作都是 weak，因此在图像加载完成后将对其释放。如果需要存储操作，请使用您自己的类为它们保留一个强引用。
@interface UIView (WebCacheOperation)

/**
 *  Get the image load operation for key
 *
 *  @param key key for identifying the operations
 *  @return the image load operation
 */
- (nullable id<SDWebImageOperation>)sd_imageLoadOperationForKey:(nullable NSString *)key;

/**
 *  Set the image load operation (storage in a UIView based weak map table)
 *
 *  @param operation the operation
 *  @param key       key for storing the operation
 */
- (void)sd_setImageLoadOperation:(nullable id<SDWebImageOperation>)operation forKey:(nullable NSString *)key;

// Cancel all operations for the current UIView and key
// 取消当前 UIView 的所有操作
- (void)sd_cancelImageLoadOperationWithKey:(nullable NSString *)key;

/**
 *  Just remove the operations corresponding to the current UIView and key without cancelling them
 *
 *  @param key key for identifying the operations
 */
- (void)sd_removeImageLoadOperationWithKey:(nullable NSString *)key;

@end
