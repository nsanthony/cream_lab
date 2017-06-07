// @(#)root/pyroot:$Id: ClassMethodHolder.h 28890 2009-06-11 00:37:35Z wlav $
// Author: Wim Lavrijsen, Aug 2004

#ifndef PYROOT_TCLASSMETHODHOLDER_H
#define PYROOT_TCLASSMETHODHOLDER_H

// ROOT
class TClass;
class TMethod;

// Bindings
#include "MethodHolder.h"


namespace PyROOT {

/** Python side ROOT static function
      @author  WLAV
      @date    08/03/2004
      @version 3.0
 */

   template< class T, class M >
   class TClassMethodHolder : public TMethodHolder< T, M > {
   public:
      TClassMethodHolder( const T& klass, const M& method );

      virtual PyObject* operator()( ObjectProxy*, PyObject* args, PyObject* kwds, Long_t = 0 );
   };

} // namespace PyROOT

#endif // !PYROOT_TCLASSMETHODHOLDER_H
