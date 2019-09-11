/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDImageCoder.h"


// Global object holding the array of coders, so that we avoid passing them from object to object.
// Uses a priority queue behind scenes, which means the latest added coders have the highest priority.
// This is done so when encoding/decoding something, we go through the list and ask each coder if they can handle the current data.
// That way, users can add their custom coders while preserving our existing prebuilt ones
// 保存 coders 数组的全局对象，以便我们避免将它们从一个对象传递到另一个对象。
// 在后台使用优先级队列，这意味着最新添加的 coders 具有最高的优先级。
// 这样做是为了在编码/解码时，我们检查列表并询问每个 coders 是否能够处理当前 data。
// 这样，用户可以添加他们的自定义 coders，同时保留我们现有的 coders。
 
// Note: the `coders` getter will return the coders in their reversed order
// Example:
// - by default we internally set coders = `IOCoder`, `GIFCoder`, `APNGCoder`
// - calling `coders` will return `@[IOCoder, GIFCoder, APNGCoder]`
// - call `[addCoder:[MyCrazyCoder new]]`
// - calling `coders` now returns `@[IOCoder, GIFCoder, APNGCoder, MyCrazyCoder]`
// 注意：`coders` getter 将按相反的顺序返回 coders
// Example:
// - 默认情况下，我们在内部设置 coders = `IOCoder`, `GIFCoder`, `APNGCoder`
// - 调用 `coders` 将返回 `@[IOCoder, GIFCoder, APNGCoder]`
// - 调用 `[addCoder:[MyCrazyCoder new]]`
// - 调用 `coders` 现在返回 `@[IOCoder, GIFCoder, APNGCoder, MyCrazyCoder]`

// Coders
// ------
// A coder must conform to the `SDImageCoder` protocol or even to `SDProgressiveImageCoder` if it supports progressive decoding
// Conformance is important because that way, they will implement `canDecodeFromData` or `canEncodeToFormat`
// Those methods are called on each coder in the array (using the priority order) until one of them returns YES.
// That means that coder can decode that data / encode to that format
// coder 必须符合 `SDImageCoder` 协议，甚至 `SDProgressiveImageCoder`，如果它支持渐进式解码
// 一致性很重要，因为这样，它们将实现 `canDecodeFromData` or `canEncodeToFormat`
// 在数组中的每个 coder 上调用这些方法（使用优先级顺序），直到其中一个返回 YES。
// 这意味着 coder 可以将数据解码/编码为该格式
@interface SDImageCodersManager : NSObject <SDImageCoder>

// Returns the global shared coders manager instance.
@property (nonatomic, class, readonly, nonnull) SDImageCodersManager *sharedManager;

// All coders in coders manager. The coders array is a priority queue, which means the later added coder will have the highest priority
@property (nonatomic, copy, nullable) NSArray<id<SDImageCoder>> *coders;

// Add a new coder to the end of coders array. Which has the highest priority.
- (void)addCoder:(nonnull id<SDImageCoder>)coder;

// Remove a coder in the coders array.
- (void)removeCoder:(nonnull id<SDImageCoder>)coder;

@end
