import json
import sys
import csv
import requests as request


def get_channel_id(channel):
    virgin_url = "https://api.twitch.tv/kraken/users?login=" + \
        channel + "&_=1631225185431"
    headers = {
        'accept': 'application/vnd.twitchtv.v5+json',
        'client-ID': '8s3qw0n4yacm066p8d7d4nm94mah3s',
        'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36'
    }

    res = request.get(virgin_url, headers=headers)
    json_res = json.loads(res.text)

    if json_res['_total'] != 0:
        # print(json_res['users'][0]['_id'])
        return json_res['users'][0]['_id']
    else:
        print("Channel not found..... pepesad")
        sys.exit()


def get_followers(channel, channel_id):
    session_counter = 0
    follower_counter = 0
    finished = False

    f = open('%s.csv' % channel, 'w', encoding='utf-8', newline="\n")
    mywriter = csv.writer(f, dialect='excel', quoting=csv.QUOTE_MINIMAL)
    #mywriter.writerow(['Followed At (UTC)','Notifications?','User ID','User Name','User Created At (UTC)','User Updated At (UTC)','Display Name','Logo URL','Bio','User Type'])
    mywriter.writerow(['Followed At (UTC)', 'Notifications?', 'User ID', 'User Name',
                       'User Created At (UTC)', 'User Updated At (UTC)', 'Display Name', 'User Type'])
    url = ("https://api.twitch.tv/v5/channels/" + channel_id +
           "/follows?limit=100&direction=asc&client_id=8s3qw0n4yacm066p8d7d4nm94mah3s&_=1631049616435")
    response = request.get(url)
    myjson = json.loads(response.text)

    while (len(myjson['follows']) > 0 and finished == False):
        for follower in myjson['follows']:
            follower_counter += 1
            created_at = follower['created_at']
            notified = follower['notifications']
            user_id = follower['user']['_id']
            user_name = follower['user']['name']
            user_created = follower['user']['created_at']
            user_updated = follower['user']['updated_at']
            user_display = follower['user']['display_name']
            """
            if (follower['user']['logo'] != None):
                logo_url    = follower['user']['logo']
            else:
                logo_url = ""
            if (follower['user']['bio'] != None):
                user_bio    = follower['user']['bio']
            else:
                user_bio = ""
            """
            user_type = follower['user']['type'].encode("utf-8")
            # mywriter.writerow([str(created_at),str(notified),user_id,str(user_name),str(user_created),str(user_updated),str(user_display),str(logo_url),str(user_bio),str(user_type)])
            mywriter.writerow([str(created_at), str(notified), user_id, str(user_name), str(
                user_created), str(user_updated), str(user_display), str(user_type)])

        if '_cursor' in myjson:
            url = ("https://api.twitch.tv/v5/channels/" + channel_id +
                   "/follows?limit=100&direction=asc&client_id=8s3qw0n4yacm066p8d7d4nm94mah3s&cursor=" + myjson['_cursor'] + "&_=1631049616435")
            # print(url)
            response = request.get(url)
            myjson = json.loads(response.text)
            session_counter += 100
            print(f"Now at {session_counter} followers and counting...")
        else:
            finished = True

    f.close
    print(f"Finished at {follower_counter} followers")
    print("Finished")


if __name__ == "__main__":
    channel = input("Enter channel name: ")
    if channel.isalnum():
        channel_id = get_channel_id(channel)
        get_followers(channel, channel_id)
    else:
        print("No channel name entered / Wrong channel name format")
        sys.exit()
