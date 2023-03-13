count=$1
while [[ $count -ge 0 ]]; do
  echo $count
  curl http://127.0.0.1:8085/hello.html
  count=$(($count - 1))
done
