__author__ = 'scott'

import boto3
import random
import botocore

s3 = boto3.resource('s3')


# print out all the buckets
def print_buckets():
    for bucket in s3.buckets.all():
        print(bucket.name)


# Create a bucket
def create_bucket(bucket_name):

    # An alternative with a random int at the end
    # bucket_name = "mybucket" + random.randint(0,1000000)
    s3.create_bucket(Bucket=bucket_name)
    s3.create_bucket(Bucket=bucket_name, CreateBucketConfiguration={
        'LocationConstraint': 'us-west-1'})

# Get a bucket object with checks to see if the bucket exists
def access_bucket(bucket_name):
    bucket = s3.Bucket(bucket_name)
    exists = True
    try:
        s3.meta.client.head_bucket(Bucket=bucket_name)
    except botocore.exceptions.ClientError as e:
        # If a client error is thrown, then check that it was a 404 error.
        # If it was a 404 error, then the bucket does not exist.
        error_code = int(e.response['Error']['Code'])
        if error_code == 404:
            exists = False

    return bucket, exists


def delete_bucket(bucket):
    for object in bucket.objects.all():
        object.delete()
    bucket.delete()

