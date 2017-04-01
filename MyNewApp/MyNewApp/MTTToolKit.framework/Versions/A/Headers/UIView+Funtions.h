//
//  UIView+Funtions.h
//  MTTToolKit
//
//  Created by lyleKP on 2017/3/14.
//  Copyright © 2017年 lyle. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIView (Funtions)
/**
 Remove all subviews.
 
 @warning Never call this method inside your view's drawRect: method.
 */
- (void)removeAllSubviews;

/**
 Returns the view's view controller (may be nil).
 */
@property (nullable, nonatomic, readonly) UIViewController *viewController;

/**
 Returns the visible alpha on screen, taking into account superview and window.
 */

@end
