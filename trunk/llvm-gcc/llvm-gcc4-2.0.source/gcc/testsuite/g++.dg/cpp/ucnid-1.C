/* APPLE LOCAL begin mainline UCNs 2005-04-17 3892809 */
/* { dg-do preprocess } */
/* { dg-options "-pedantic" } */

\u00AA /* { dg-error "not valid in an identifier" } */
\u00AB /* { dg-error "not valid in an identifier" } */
\u00B6 /* { dg-error "not valid in an identifier" } */
\u00BA /* { dg-error "not valid in an identifier" } */
\u00C0
\u00D6
\u0384

\u0669 /* { dg-error "not valid in an identifier" } */
A\u0669 /* { dg-error "not valid in an identifier" } */
0\u00BA /* { dg-error "not valid in an identifier" } */
0\u0669 /* { dg-error "not valid in an identifier" } */
\u0E59
A\u0E59
/* APPLE LOCAL end mainline UCNs 2005-04-17 3892809 */
