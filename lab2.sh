#!/bin/bash
echo $(journalctl | grep -P 'kernel.*USB.[23]')

