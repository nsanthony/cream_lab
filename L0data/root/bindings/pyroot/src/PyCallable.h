// @(#)root/pyroot:$Id: PyCallable.h 28890 2009-06-11 00:37:35Z wlav $
// Author: Wim Lavrijsen, Aug 2004

#ifndef PYROOT_PYCALLABLE_H
#define PYROOT_PYCALLABLE_H


namespace PyROOT {

/** Python callable object interface
      @author  WLAV
      @date    01/27/2009
      @version 4.0
 */

   class ObjectProxy;

   class PyCallable {
   public:
      virtual ~PyCallable() {}

   public:
      virtual PyObject* GetSignature() = 0;
      virtual PyObject* GetPrototype() = 0;
      virtual PyObject* GetDocString() { return GetPrototype(); }
      virtual Int_t GetPriority() { return 0; }

      virtual Int_t GetMaxArgs() { return 0; }
      virtual PyObject* GetArgSpec( Int_t /* iarg */ ) { return 0; }
      virtual PyObject* GetArgDefault( Int_t /* iarg */ ) { return 0; }
      virtual PyObject* GetScope() { return 0; }

   public:
      virtual PyObject* operator()(
         ObjectProxy* self, PyObject* args, PyObject* kwds, Long_t user = 0 ) = 0;
   };

} // namespace PyROOT

#endif // !PYROOT_PYCALLABLE_H