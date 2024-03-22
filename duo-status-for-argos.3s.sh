#!/usr/bin/env bash

URL="github.com/p-e-w/argos"
DIR=$(dirname "$0")

# $HOME/p/my-scripts/duo set-displays > /dev/null 2>/dev/null
$HOME/p/my-scripts/duo status
echo "---"
echo "top    | terminal=false bash='duo top'"
echo "both   | terminal=false bash='duo both'"
echo "bottom | terminal=false bash='duo bottom'"
echo "---"
echo "$URL | iconName=help-faq-symbolic href='https://$URL'"
echo "$DIR | iconName=folder-symbolic href='file://$DIR'"

