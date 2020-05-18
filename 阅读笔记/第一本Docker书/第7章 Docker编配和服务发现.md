## Docker Compose

docker-compose.yml

```yaml
version: '3'
services:
  web:
    image: jamtur01/composeapp
    command: python app.py
    ports:
     - "5000:5000"
    volumes:
     - .:/composeapp
  redis:
    image: redis
```

```shell
$ sudo docker-compose up
$ sudo docker-compose up -d
$ sudo docker-compose ps
$ sudo docker-compose logs
$ sudo docker-compose stop
$ sudo docker-compose rm
```

## Consul、服务发现与Docker

## 集群和调度工具 Docker Swarm