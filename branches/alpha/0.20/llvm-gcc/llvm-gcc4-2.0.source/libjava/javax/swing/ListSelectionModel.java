/* ListSelectionModel.java -- 
   Copyright (C) 2002 Free Software Foundation, Inc.

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


package javax.swing;

import javax.swing.event.ListSelectionListener;

public interface ListSelectionModel
{
  int SINGLE_SELECTION = 0;
  int SINGLE_INTERVAL_SELECTION = 1;
  int MULTIPLE_INTERVAL_SELECTION = 2;

  void setSelectionMode(int a);
  int getSelectionMode();
  
  void clearSelection();
    
  int getMinSelectionIndex();
  int getMaxSelectionIndex();

  boolean isSelectedIndex(int a);

  boolean isSelectionEmpty();
  void setSelectionInterval(int index0, int index1);
  void addSelectionInterval(int index0,
                            int index1);
  void removeSelectionInterval(int index0,
                               int index1);
  void insertIndexInterval(int index,
                           int length,
                           boolean before);
  void removeIndexInterval(int index0,
                           int index1);

  int getAnchorSelectionIndex();
  void setAnchorSelectionIndex(int index);
  int getLeadSelectionIndex();
  void setLeadSelectionIndex(int index);

  void setValueIsAdjusting(boolean valueIsAdjusting);
  boolean getValueIsAdjusting();

  void addListSelectionListener(ListSelectionListener listener);
  void removeListSelectionListener(ListSelectionListener listener);    

}
