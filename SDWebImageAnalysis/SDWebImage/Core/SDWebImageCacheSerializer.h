/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSData * _Nullable(^SDWebImageCacheSerializerBlock)(UIImage * _Nonnull image, NSData * _Nullable data, NSURL * _Nullable imageURL);

// This is the protocol for cache serializer.
// We can use a block to specify the cache serializer. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
// 缓存序列化器的协议。
// 我们可以使用 block 来指定缓存序列化器。但是使用协议可以使这个扩展，并允许 Swift 用户轻松使用它，而不是使用 `@convention(block)` 将 block 存储到上下文选项中。
@protocol SDWebImageCacheSerializer <NSObject>

- (nullable NSData *)cacheDataWithImage:(nonnull UIImage *)image originalData:(nullable NSData *)data imageURL:(nullable NSURL *)imageURL;

@end

/**
 A cache serializer class with block.
 */
@interface SDWebImageCacheSerializer : NSObject <SDWebImageCacheSerializer>

- (nonnull instancetype)initWithBlock:(nonnull SDWebImageCacheSerializerBlock)block;
+ (nonnull instancetype)cacheSerializerWithBlock:(nonnull SDWebImageCacheSerializerBlock)block;

@end
