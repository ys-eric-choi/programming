# -*- coding: utf-8 -*-
import flask

application = flask.Flask(__name__)

@application.route("/")
def hello():
	# 소스 코드와 동일 위치에 templates 디렉토리 생성	
	# templates 디렉토리 내에 html 파일 작성
	return flask.render_template('index.html')

if __name__ == "__main__":
	application.run(host='0.0.0.0')
