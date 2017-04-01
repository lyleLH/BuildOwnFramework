//
//  UIView+DataOperation.h
//  MTTToolKit
//
//  Created by lyleKP on 2017/3/14.
//  Copyright © 2017年 lyle. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIView (DataOperation)
/**
 Create a snapshot image of the complete view hierarchy.
 */
- (nullable UIImage *)snapshotImage;

/**
 Create a snapshot image of the complete view hierarchy.
 @discussion It's faster than "snapshotImage", but may cause screen updates.
 See -[UIView drawViewHierarchyInRect:afterScreenUpdates:] for more information.
 */
- (nullable UIImage *)snapshotImageAfterScreenUpdates:(BOOL)afterUpdates;

/**
 Create a snapshot PDF of the complete view hierarchy.
 */
- (nullable NSData *)snapshotPDF;

@end
