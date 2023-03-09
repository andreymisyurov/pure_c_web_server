path=$(pwd)/.clang-format

function traverse {
  for dir in */; do
    if [ -d "$dir" ] && [ $dir != "templates" ]; then
      echo "Running script in directory: $dir"
      cd "$dir"
      cp $path .
      clang-format -i --sort-includes --verbose --style="Google" *.c
      clang-format -i --sort-includes --verbose --style="Google" *.h
      if [ "$(ls -A)" ]; then
        traverse
      fi
      rm .clang-format
      cd ..
    fi
  done
}

traverse
make build
if [ $? = 0 ]; then
  git commit -m\""$1"\"
  git push origin main
fi

rm server.out
