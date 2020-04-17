/* typemaps for librat */

/* Input arrays for various functions */
%typemap(in) double *from, double *direction, double *minwavelength, double *bandwidth {
   int i,size;
   size=PyList_Size($input);
   $1=(double *)malloc(sizeof(double)*size);
   for(i=0;i<size;i++){
      $1[i]=PyFloat_AsDouble(PyList_GetItem($input,i));
   }
}

/*
RATgetMaterials output
*/
%typemap(in, numinputs=0) int *nMat (int temp) {
   $1 = &temp;
}
%typemap(argout) int *nMat {
   PyObject * scalar = PyInt_FromLong(*$1);
   $result = SWIG_Python_AppendOutput($result,scalar);
}

/*
RATtranslateMaterial output
*/
%typemap(in, numinputs=0) int *index (int temp) {
   $1 = &temp;
}
%typemap(argout) int *index {
   PyObject * scalar = PyInt_FromLong(*$1);
   $result = SWIG_Python_AppendOutput($result,scalar);
}

/*
RATgetRatTree output
Structure is currently returned as a list of numpy arrays
Would be better if it returned as an object, but have not worked that out yet
*/
%typemap (out) RATtree * {
    {  
        PyObject * scalar = PyInt_FromLong($1->n);
        $result = SWIG_Python_AppendOutput($result,scalar);
    }
        {  
        PyObject * scalar = PyInt_FromLong($1->thisRTD);
        $result = SWIG_Python_AppendOutput($result,scalar);
    }
    {  
        npy_intp dims[2] = { $1->n , 3 };
        PyObject * array = PyArray_SimpleNewFromData(2, dims, NPY_DOUBLE, (void*)(*$1->intersectionPoints));
            if (!array) SWIG_fail;
        $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void*)($1->rayLengths));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_INTP, (void*)($1->ratmat));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_INTP, (void*)($1->diffusePathIntersectType));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_INTP, (void*)($1->directPathIntersectType));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_INTP, (void*)($1->hitSun));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_INTP, (void*)($1->hitSky));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void*)($1->diffuseRadiance));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }    
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void*)($1->directRadiance));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void*)($1->lengthToSun));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }    
        {
        npy_intp dims[1] = { $1->n };           
        PyObject * array = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void*)($1->angleToSun));
                if (!array) SWIG_fail;
                $result = SWIG_Python_AppendOutput($result,array);
    }
}
