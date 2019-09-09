/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

typedef NSURLRequest * _Nullable (^SDWebImageDownloaderRequestModifierBlock)(NSURLRequest * _Nonnull request);

// This is the protocol for downloader request modifier.
// We can use a block to specify the downloader request modifier. But Using protocol can make this extensible, and allow Swift user to use it easily instead of using `@convention(block)` to store a block into context options.
// 这是 downloader 请求修饰符的协议。
// 我们可以使用 block 来指定 downloader 请求修饰符。但是使用协议可以使这个可扩展，并允许 Swift 用户轻松使用它，而不是使用 `@convention(block)` 将 block 存储到上下文选项中。
@protocol SDWebImageDownloaderRequestModifier <NSObject>

- (nullable NSURLRequest *)modifiedRequestWithRequest:(nonnull NSURLRequest *)request;

@end

// A downloader request modifier class with block.
@interface SDWebImageDownloaderRequestModifier : NSObject <SDWebImageDownloaderRequestModifier>

- (nonnull instancetype)initWithBlock:(nonnull SDWebImageDownloaderRequestModifierBlock)block;
+ (nonnull instancetype)requestModifierWithBlock:(nonnull SDWebImageDownloaderRequestModifierBlock)block;

@end
