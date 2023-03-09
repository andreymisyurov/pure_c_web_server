#
# Скрипт для пуша на репозиторий. Выравнивает код по Google-style.
# Не дает запушить код, который не компилируется
#

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
  echo $?
  git commit -m\""$1"\"
  git push origin main
else
  echo "НЕ МОГУ ЗАПУШИТЬ КОД КОТОРЫЙ НЕ КОМПИЛИРУЕТСЯ"
fi

rm server.out
