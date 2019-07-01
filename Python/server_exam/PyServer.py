#from flask import Flask
from flask import Flask, make_response, request, current_app, jsonify
app = Flask(__name__)

#@app.route('/')
@app.route('/_get_data/', methods=['GET'])
#@app.route('/_get_data/', methods=['POST'])
def _get_data():

    # POST
    #strIn  = request.form['in']
    # GET
    strIn = request.args.get('in')
    print(strIn)

    dictResp = {}
    dictResp[strIn] = "Hello"

    #return 'Hello World!'
    return jsonify(dictResp)

if __name__ == '__main__':
    app.run()
