# SDWebImage 源码解析（更新至 5.1 版本）

[SDWebImage](https://github.com/SDWebImage/SDWebImage) 主要包含三个功能模块：下载（SDWebImageDownloader）、编码/解码（SDImageCodersManager）、缓存策略（SDImageCache）

> https://www.cnblogs.com/LeeGof/p/6914267.html
> https://www.cnblogs.com/machao/p/6265621.html
> http://www.cocoachina.com/cms/wap.php?action=article&id=22843
> https://my.oschina.net/starmier/blog/467251

### 知识点 1：FOUNDATION_EXPORT 和 #define

FOUNDATION_EXPORT 与 #define 都是用来定义常量的，前者比较时是用的是指针，后者是比较每一个字符，在使用宏定义多的地方可以考虑使用 FOUNDATION_EXPORT 效率会变高。

### 知识点 2：下载器

SDWebImageDownloader 完成了对网络图片的异步下载工作，准确说这个类是一个文件下载的工具类，真正的网络请求是在继承于 NSOperation 的 SDWebImageDownloaderOperation 类实现的。SDWebImageDownloader 的主要任务是下载相关配置项的管理，包括下载队列的先后顺序、最大下载任务数量控制、下载队列中的任务创建、取消、暂停等任务管理，以及其他 HTTPS 和 HTTP Header 的设置。

### 问题 1：SDMemoryCache 中的 weakCache 作用？

weakCache 是一个 NSMapTable 对象，通过弱引用来持有 keys 和 values，所以当 key 或者 value 被释放的时候，所存储的实体也会被移除。

```objc
self.weakCache = [[NSMapTable alloc] initWithKeyOptions:NSPointerFunctionsStrongMemory valueOptions:NSPointerFunctionsWeakMemory capacity:0];
```

既然 NSCache 已经可以实现图片的内存缓存了，为啥还要加一个 NSMapTable 再缓存一次呢？可能是因为 NSCache 在收到内存警告时会自动释放缓存，但是它的释放是没有顺序的，所以可能是刚存入的数据对象被清理了，而不是我们希望的「先进先出」顺序，在实际情况中，往往是最新存入的数据被再次用到的可能性比较大，所以作者在 NSCache 的基础上又加了一个 NSMapTable 缓存，这应该是为了提高内存缓存的命中率吧。

我们再来看一下具体的代码实现，作者重写了 NSCache 的方法来实现 NSMapTable 的缓存，为了方便阅读，我删除了线程安全的代码：

```objc
- (void)setObject:(id)obj forKey:(id)key cost:(NSUInteger)g {
    [super setObject:obj forKey:key cost:g];
    if (key && obj) {
        [self.weakCache setObject:obj forKey:key];
    }
}

- (id)objectForKey:(id)key {
    id obj = [super objectForKey:key];
    if (key && !obj) {
        obj = [self.weakCache objectForKey:key];
        if (obj) {
            NSUInteger cost = 0;
            if ([obj isKindOfClass:[UIImage class]]) {
                cost = [(UIImage *)obj sd_memoryCost];
            }
            [super setObject:obj forKey:key cost:cost];
        }
    }
    return obj;
}

- (void)removeObjectForKey:(id)key {
    [super removeObjectForKey:key];
    if (key) {
        [self.weakCache removeObjectForKey:key];
    }
}

- (void)removeAllObjects {
    [super removeAllObjects];
    [self.weakCache removeAllObjects];
}
```
