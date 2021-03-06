/* AccessibleHypertext.java -- aids in accessibly rendering hypertext
   Copyright (C) 2000, 2002, 2005  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


package javax.accessibility;

/**
 * Objects which present hyperlinks in a document should implement this
 * interface.  Accessibility software can use the implementations of this
 * interface to aid the user in navigating the links.
 *
 * <p>The <code>AccessibleContext.getAccessibleText()</code> method
 * should return an instance of this interface only when it is supported.
 *
 * @author Eric Blake (ebb9@email.byu.edu)
 * @see Accessible
 * @see AccessibleContext
 * @see AccessibleText
 * @see AccessibleContext#getAccessibleText()
 * @since 1.2
 * @status updated to 1.4
 */
public interface AccessibleHypertext extends AccessibleText
{
  /**
   * Returns the number of links in the document, if any exist.
   *
   * @return the number of links, or -1
   */
  int getLinkCount();

  /**
   * Returns link object denoted by the number <code>i</code> in this
   * document, or null if i is out of bounds.
   *
   * @param i the ith hyperlink of the document
   * @return link object denoted by <code>i</code>
   */
  AccessibleHyperlink getLink(int i);

  /**
   * Returns the link index for this character index if it resides within
   * one of the hyperlinks of the document. If no association exists at that
   * character, or c is out of bounds, returns -1.
   *
   * @param c the character index
   * @return the link index, or -1
   */
  int getLinkIndex(int c);
} // interface AccessibleHypertext
