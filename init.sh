#!/bin/bash

cd peanuts/;
git submodule init;
git submodule update;
perl glload.pl;
perl peanuts.pl;
cd ..;
