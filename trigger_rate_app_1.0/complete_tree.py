#! /home/nsanthony/anaconda3/bin/python

from get_tree import get_tree
import numpy as np

def complete_tree(file_list=None):
    if len(file_list) == 1:
        tree = get_tree(file_list=file_list[0])
    else:
        for i in range(0,len(file_list)):
            if i == 0:
                tree = get_tree(file_list=file_list[i])
            else:
               tree1 = get_tree(file_list=file_list[i])
               tree = np.append(tree,tree1)
       
    return tree 