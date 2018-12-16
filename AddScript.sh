#!/bin/bash
echo "Add Script"
IFS="
"
read -p "Enter Script Path: " path
read -p "Enter WebSite: " site
cp $path "$(./GetBasePath)/${site}.load.js"
echo "Installed Script"
