#!/usr/bin/env bash

CMD=clang-format-7
$CMD -version
$CMD -i -style=Google $(git ls-files|grep -E ".*\.(cc|h)$")
CHANGED="$(git ls-files --modified)"
if [[ ! -z "$CHANGED" ]]; then
  echo "The following files have changes due to incrrect format:"
  echo "$CHANGED"
  echo "please use format.sh script fix it"
  exit 1
else
  echo "No changes."
fi
