import argparse
import getpass

class Password(argparse.Action):
    def __call__(self, parser, namespace, values, option_string):
        if values is None:
            values = getpass.getpass()
        setattr(namespace, self.dest, values)

parser = argparse.ArgumentParser()
parser.add_argument('-p', action=Password, nargs='?', dest='password')
args = parser.parse_args()

password = args.password
print password