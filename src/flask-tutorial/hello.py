import flaskr


if __name__ == '__main__':
    app = flaskr.create_app()
    app.run(host='127.0.0.1', port=9000, debug=True, threaded=True)