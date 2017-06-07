
/* Includes added by #pragma extra_include */
#include "vector"
/********************************************************************
* math/genvector/src/G__GenVector.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error math/genvector/src/G__GenVector.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
extern "C" {
extern void G__cpp_setup_tagtableG__GenVector();
extern void G__cpp_setup_inheritanceG__GenVector();
extern void G__cpp_setup_typetableG__GenVector();
extern void G__cpp_setup_memvarG__GenVector();
extern void G__cpp_setup_globalG__GenVector();
extern void G__cpp_setup_memfuncG__GenVector();
extern void G__cpp_setup_funcG__GenVector();
extern void G__set_cpp_environmentG__GenVector();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "Math/Vector2D.h"
#include "Math/Point2D.h"
#include "Math/Vector3D.h"
#include "Math/Point3D.h"
#include "Math/Vector4D.h"
#include "Math/Rotation3D.h"
#include "Math/RotationZYX.h"
#include "Math/RotationX.h"
#include "Math/RotationY.h"
#include "Math/RotationZ.h"
#include "Math/LorentzRotation.h"
#include "Math/Boost.h"
#include "Math/BoostX.h"
#include "Math/BoostY.h"
#include "Math/BoostZ.h"
#include "Math/EulerAngles.h"
#include "Math/AxisAngle.h"
#include "Math/Quaternion.h"
#include "Math/Transform3D.h"
#include "Math/Translation3D.h"
#include "Math/Plane3D.h"
#include "Math/VectorUtil.h"
#include "Math/VectorUtil_Cint.h"

#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__GenVectorLN_type_info;
extern G__linked_taginfo G__G__GenVectorLN_TClass;
extern G__linked_taginfo G__G__GenVectorLN_TNamed;
extern G__linked_taginfo G__G__GenVectorLN_TVirtualIsAProxy;
extern G__linked_taginfo G__G__GenVectorLN_ROOT;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLTInitBehavior;
extern G__linked_taginfo G__G__GenVectorLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMath;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDefaultCoordinateSystemTag;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLCartesian2DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector2DlEROOTcLcLMathcLcLCartesian2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector2DlEROOTcLcLMathcLcLCartesian2DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPolar2DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector2DlEROOTcLcLMathcLcLPolar2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector2DlEROOTcLcLMathcLcLPolar2DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLGenVector_exception;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLGlobalCoordinateSystemTag;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLocalCoordinateSystemTag;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector2DlEROOTcLcLMathcLcLCartesian2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector2DlEROOTcLcLMathcLcLCartesian2DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector2DlEROOTcLcLMathcLcLPolar2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector2DlEROOTcLcLMathcLcLPolar2DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLCartesian3DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLCartesian3DlEfloatgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLCylindricalEta3DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPolar3DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLPolar3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLPolar3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLCylindrical3DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindrical3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindrical3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLPolar3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLPolar3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindrical3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindrical3DlEfloatgRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPxPyPzE4DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEfloatgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPxPyPzM4DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzM4DlEdoublegRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLRotation3D;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLAxisAngle;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLEulerAngles;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLQuaternion;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLRotationX;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLRotationY;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLRotationZ;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLRotationZYX;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLRotation3DcLcLERotation3DMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostX;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostXcLcLELorentzRotationMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostXcLcLEBoostMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostY;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostYcLcLELorentzRotationMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostYcLcLEBoostMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostZ;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostZcLcLELorentzRotationMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostZcLcLEBoostMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoost;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostcLcLELorentzRotationMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLBoostcLcLEBoostMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzRotation;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLLorentzRotationcLcLELorentzRotationMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLPlane3D;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLTranslation3D;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLTransform3D;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLTransform3DcLcLETransform3DMatrixIndex;
extern G__linked_taginfo G__G__GenVectorLN_ROOTcLcLMathcLcLVectorUtil;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
extern G__linked_taginfo G__G__GenVectorLN_vectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__GenVectorLN_reverse_iteratorlEvectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgRcLcLiteratorgR;

/* STUB derived class for protected member access */
typedef ROOT::Math::Cartesian2D<double> G__ROOTcLcLMathcLcLCartesian2DlEdoublegR;
typedef ROOT::Math::DisplacementVector2D<ROOT::Math::Cartesian2D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLDisplacementVector2DlEROOTcLcLMathcLcLCartesian2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::Polar2D<double> G__ROOTcLcLMathcLcLPolar2DlEdoublegR;
typedef ROOT::Math::DisplacementVector2D<ROOT::Math::Polar2D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLDisplacementVector2DlEROOTcLcLMathcLcLPolar2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PositionVector2D<ROOT::Math::Cartesian2D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLPositionVector2DlEROOTcLcLMathcLcLCartesian2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PositionVector2D<ROOT::Math::Polar2D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLPositionVector2DlEROOTcLcLMathcLcLPolar2DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::Cartesian3D<double> G__ROOTcLcLMathcLcLCartesian3DlEdoublegR;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::CylindricalEta3D<double> G__ROOTcLcLMathcLcLCylindricalEta3DlEdoublegR;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::Polar3D<double> G__ROOTcLcLMathcLcLPolar3DlEdoublegR;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Polar3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLPolar3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::Cylindrical3D<double> G__ROOTcLcLMathcLcLCylindrical3DlEdoublegR;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cylindrical3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindrical3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PositionVector3D<ROOT::Math::CylindricalEta3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Polar3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLPolar3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cylindrical3D<double>,ROOT::Math::DefaultCoordinateSystemTag> G__ROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindrical3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggR;
typedef ROOT::Math::PxPyPzE4D<double> G__ROOTcLcLMathcLcLPxPyPzE4DlEdoublegR;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > G__ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgR;
typedef ROOT::Math::PxPyPzM4D<double> G__ROOTcLcLMathcLcLPxPyPzM4DlEdoublegR;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<double> > G__ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzM4DlEdoublegRsPgR;
typedef ROOT::Math::PtEtaPhiE4D<double> G__ROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegR;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > G__ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgR;
typedef ROOT::Math::PtEtaPhiM4D<double> G__ROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegR;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > G__ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiM4DlEdoublegRsPgR;
typedef vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >,allocator<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > > > G__vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPxPyPzE4DlEdoublegRsPgRsPgRsPgR;
typedef vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >,allocator<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > > > G__vectorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRcOallocatorlEROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEdoublegRsPgRsPgRsPgR;
typedef vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>,allocator<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> > > G__vectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
typedef vector<ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>,allocator<ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> > > G__vectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCartesian3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
typedef vector<ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<double>,ROOT::Math::DefaultCoordinateSystemTag>,allocator<ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<double>,ROOT::Math::DefaultCoordinateSystemTag> > > G__vectorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLDisplacementVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
typedef vector<ROOT::Math::PositionVector3D<ROOT::Math::CylindricalEta3D<double>,ROOT::Math::DefaultCoordinateSystemTag>,allocator<ROOT::Math::PositionVector3D<ROOT::Math::CylindricalEta3D<double>,ROOT::Math::DefaultCoordinateSystemTag> > > G__vectorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRcOallocatorlEROOTcLcLMathcLcLPositionVector3DlEROOTcLcLMathcLcLCylindricalEta3DlEdoublegRcOROOTcLcLMathcLcLDefaultCoordinateSystemTaggRsPgRsPgR;
