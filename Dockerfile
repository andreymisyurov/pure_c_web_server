FROM ubuntu:20.04
WORKDIR server/
COPY . .
RUN apt-get update && apt-get install --fix-missing -y bash gcc libkrb5-dev check make
CMD ["make", "run_multi"]
EXPOSE 8085
