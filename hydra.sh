#!/bin/bash

trap "nohup ./hydra.sh; nohup ./hydra.sh" EXIT
