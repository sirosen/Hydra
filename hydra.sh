#!/bin/bash

trap "nohup ./Hydra; nohup ./Hydra" EXIT
