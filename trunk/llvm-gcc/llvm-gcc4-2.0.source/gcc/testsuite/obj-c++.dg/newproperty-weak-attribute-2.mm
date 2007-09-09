/* APPLE LOCAL file radar 4805321 */
/* Test a variety of error reporting on mis-use of 'weak' attribute */
/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-mmacosx-version-min=10.5 -fobjc-new-property -fobjc-gc" } */

@interface INTF
{
  id IVAR;
}
@property (assign) __weak id pweak;
@end	

@implementation INTF
@synthesize pweak=IVAR; /* { dg-error "existing ivar 'IVAR' for the '__weak' property 'pweak' must be __weak" } */
@end