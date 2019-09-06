/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDImageLoader.h"

// A loaders manager to manage multiple loaders
// 一个管理多个加载器的 loaders manager
@interface SDImageLoadersManager : NSObject <SDImageLoader>

// Returns the global shared loaders manager instance. By default we will set [`SDWebImageDownloader.sharedDownloader`] into the loaders array.
// 返回全局共享 loaders manager 实例。默认情况下，我们将 [`SDWebImageDownloader.sharedDownloader`] 设置为 loaders 数组。
@property (nonatomic, class, readonly, nonnull) SDImageLoadersManager *sharedManager;

// All image loaders in manager. The loaders array is a priority queue, which means the later added loader will have the highest priority
// manager 中的所有图像 loaders。loaders 数组是优先级队列，这意味着后来添加的 loader 将具有最高优先级
@property (nonatomic, copy, nullable) NSArray<id<SDImageLoader>>* loaders;

// Add a new image loader to the end of loaders array. Which has the highest priority.
// 将新的图像 loader 添加到 loaders 数组的末尾。其优先级最高。
- (void)addLoader:(nonnull id<SDImageLoader>)loader;

// Remove a image loader in the loaders array.
// 删除 loaders 数组中的图像 loader。
- (void)removeLoader:(nonnull id<SDImageLoader>)loader;

@end
