#ifndef UIKIT_H
#define UIKIT_H
#import <CoreGraphics/CoreGraphics.h>

/* Don't add files to this unless you've made sure they compile. --pcwalton */

#import <Foundation/Foundation.h>
#import <UIKit/CDStructures.h>
#import <UIKit/UIAlertSheet.h>
#import <UIKit/UIApplication.h>
#import <UIKit/UIHardware.h>
#import <UIKit/UIImage.h>
#import <UIKit/UIImageAndTextTableCell.h>
#import <UIKit/UIImageView.h>
#import <UIKit/UINavigationBar.h>
#import <UIKit/UITable.h>
#import <UIKit/UITableColumn.h>
#import <UIKit/UITextLabel.h>
#import <UIKit/UIView.h>
#import <UIKit/UIView-Hierarchy.h>
#import <UIKit/UIView-Internal.h>
#import <UIKit/UIView-Rendering.h>
#import <UIKit/UIWindow.h>

int UIApplicationMain(int argc, char **argv, id Class);
CGContextRef UICurrentContext();

#endif

