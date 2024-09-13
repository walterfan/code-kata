#refer to https://github.com/hackersandslackers/paramiko-tutorial
import paramiko

def execute_ssh_command(hostname, username, password, command):
    try:
        # Create a new SSH client
        client = paramiko.SSHClient()

        # Automatically add the server's host key
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

        # Connect to the server
        client.connect(hostname, username=username, password=password)

        # Execute the command
        stdin, stdout, stderr = client.exec_command(command)
        
        # Read the output of the command
        cout = stdout.read().decode('utf-8')
        cerr = stderr.read().decode('utf-8')
        # Print the output
        return cout, cerr

    except paramiko.AuthenticationException:
        print("Authentication failed, please verify your credentials")
    except paramiko.SSHException as e:
        print(f"Unable to establish SSH connection: {e}")
    finally:
        # Close the connection
        client.close()
    return ("", "")

