#! /Users/nsanthony/miniconda3/bin/python
import os
import numpy as np
from numpy import genfromtxt
from channel_prep import prep

class data_set:
    """This is a data set object with attributes data,name,variables"""
    def data():
        """This is the data set"""
        d = 0
        return d
    def name():
        """This is the name of the detector used"""
        n = 'name of detector'
        return n
    def variables():
        """This describes the column units"""
        v = 'structure'
        return v


def acquisition(detector,path=None,time=None,date=None,layer=None):
    """Date is in format YEARMONTHDAY example: 20170524 is May 24 2017, time 
    is in format HOURMINUTESECOND example: 145342 is 14:53.42"""
    if path == None:
        path = '/Users/nsanthony/cream/cal'
    path_root = path
    path = '%s/files/temp_files' % (path_root)
    os.chdir(path)
        #checks to see what detector and type you want and assigns to the detector
        #variable. 
    print(detector,path_root,date,time,layer)
    prep(detector=detector,date=date,time=time,layer=layer,path_root=path_root)
    datetime = '%s-%s' % (date,time)
    file = '%s-%s-%s.data' % (detector,datetime,layer)
    det = genfromtxt(file,delimiter=",")
    data_set.data = det
    data_set.name = '%s_%s' % (detector,layer)
    
    return data_set