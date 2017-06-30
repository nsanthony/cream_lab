#! /home/nsanthony/anaconda3/bin/python

import sys
import ROOT
import numpy as np
from root_numpy import root2array, tree2array
import os

def get_tree(file_list):
    data_path = '/home/nsanthony/cream/data/dat/trigger_rate_data'
    os.chdir(data_path)
    part= file_list.partition('cdps2/cream/')
    current_file = part[2][len(part[2])-20:len(part[2])-5] + '.root'
    rfile = ROOT.TFile(current_file)
    intree = rfile.Get('event')
#    intree.Print()
#    print()
    event_tree = tree2array(intree,branches=['evt','time','trig','cal'])
    return event_tree
