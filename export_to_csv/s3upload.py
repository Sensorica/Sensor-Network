import boto.s3
import config_file #This is a file that stores our API keys and will not be pushed to git for obvious reasons


conn = boto.connect_s3(aws_access_key_id=config_file.AWS_ACCESS_KEY,
                       aws_secret_access_key=config_file.AWS_SECRET_KEY)

def percent_cb(complete,total):
    print ('.')

def upload_to_s3_bucket_path(bucketname,path,filename):
    mybucket = conn.get_bucket(bucketname)
    fullkeyname=os.path.join(path,filename)
    key = mybucket.new_key(fullkeyname)
    key.set_contents_from_filename(filename,cb=percent_cb,num_cb=10)

def upload_to_s3_bucket_root(bucketname,filename):
    mybucket = conn.get_bucket(bucketname)
    key = mybucket.new_key(filename)
    key.set_contents_from_filename(filename,cb=percent_cb,num_cb=10)

upload_to_s3_bucket_root('mltest12345','banking-batch.csv')


