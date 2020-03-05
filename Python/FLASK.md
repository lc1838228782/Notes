# FLASK NOTE

## Virtual environment(`DEPRECATED`)

### Create an environment

```bash
mkdir myproject
cd myproject
python3 -m venv venv
```
### Activate the environment

` . venv/bin/activate `

## Pipenv
包管理

## Run the application

```bash
export FLASK_APP=hello.py
flask run
```

### Debug mode

Three things it does:

1. it activates the debugger
2. it activates the automatic reloader
3. it enables the debug mode on the Flask application.

#### Enable the mode

` export  FLASK_ENV=development `

or `export FLASK_DEBUG=1 `

