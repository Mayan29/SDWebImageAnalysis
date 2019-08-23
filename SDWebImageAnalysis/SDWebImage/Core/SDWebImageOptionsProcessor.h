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

@class SDWebImageOptionsResult;

typedef SDWebImageOptionsResult * _Nullable(^SDWebImageOptionsProcessorBlock)(NSURL * _Nullable url, SDWebImageOptions options, SDWebImageContext * _Nullable context);

// The options result contains both options and context.
// 包含选项和上下文的选项结果
@interface SDWebImageOptionsResult : NSObject

/**
 WebCache options.
 */
@property (nonatomic, assign, readonly) SDWebImageOptions options;

/**
 Context options.
 */
@property (nonatomic, copy, readonly, nullable) SDWebImageContext *context;

/**
 Create a new options result.

 @param options options
 @param context context
 @return The options result contains both options and context.
 */
- (nonnull instancetype)initWithOptions:(SDWebImageOptions)options context:(nullable SDWebImageContext *)context;

@end

// This is the protocol for options processor.
// Options processor can be used, to control the final result for individual image request's `SDWebImageOptions` and `SDWebImageContext`
// Implements the protocol to have a global control for each indivadual image request's option.
// 这是选项处理器的协议
// 选项处理器可用于控制单个图像请求的 `SDWebImageOptions` 和 `SDWebImageContext` 的最终结果。
// 实现协议以对每个单独的图像请求选项进行全局控制。
@protocol SDWebImageOptionsProcessor <NSObject>

// Return the processed options result for specify image URL, with its options and context
// @param url The URL to the image
// @param options A mask to specify options to use for this request
// @param context A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
// @return The processed result, contains both options and context
// 返回指定图像 URL 的已处理选项结果及其选项和上下文
// @param url 图像的URL
// @param options 指定用于此请求的选项
// @param context 包含执行指定更改或进程的不同选项的上下文，请参见 `SDWebImageContextOption`。这将保存 `options` 枚举不能容纳的额外对象。
// @return 处理的结果，包含选项和上下文
- (nullable SDWebImageOptionsResult *)processedResultForURL:(nullable NSURL *)url
                                                    options:(SDWebImageOptions)options
                                                    context:(nullable SDWebImageContext *)context;

@end

/**
 A options processor class with block.
 */
@interface SDWebImageOptionsProcessor : NSObject<SDWebImageOptionsProcessor>

- (nonnull instancetype)initWithBlock:(nonnull SDWebImageOptionsProcessorBlock)block;
+ (nonnull instancetype)optionsProcessorWithBlock:(nonnull SDWebImageOptionsProcessorBlock)block;

@end
