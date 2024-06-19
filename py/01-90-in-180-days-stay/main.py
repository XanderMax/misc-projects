import datetime
import argparse
import json


def parse_trips(trips_json):
    trips = []
    for trip in trips_json:
        beg = datetime.datetime.strptime(trip["beg"], "%d.%m.%Y")
        end = datetime.datetime.strptime(trip["end"], "%d.%m.%Y")
        trips.append({"beg": beg, "end": end})
    return trips


def get_date_list(trips):
    date_list = []

    for d in trips:
        d_beg = d["beg"]
        d_end = d["end"]
        if d_beg > d_end:
            raise AssertionError(f"End date is before the beginning date - {d_beg} > {d_end}")
        while d_beg <= d_end:
            date_list.append(d_beg)
            d_beg += datetime.timedelta(days=1)
    return date_list


def process_trips(trip_dates):
    trip_dates.sort(key=lambda x: x["beg"])
    date_list = get_date_list(trip_dates)
    result = []
    for d in trip_dates:
        date_end = d["end"]
        date_beg = d["beg"]
        date_180_days_ago = date_end - datetime.timedelta(days=180)
        trip_days = (date_end - date_beg).days + 1
        days_during_180_window = 0
        for date in date_list:
            if date_180_days_ago <= date <= date_end:
                days_during_180_window += 1
        result.append({'beg': date_beg, 'end': date_end, 'days': days_during_180_window, 'trip_days': trip_days})
    for rec in result:
        fmt = "%d.%m.%Y"
        print(f"{rec['beg'].strftime(fmt)} \t {rec['end'].strftime(fmt)} \t {rec['trip_days']} \t {rec['days']}")

    
def test():
    dates = [
        {"beg":"24.06.2016", "end":"06.07.2016"},
        {"beg":"03.10.2016", "end":"17.10.2016"},
        {"beg":"05.01.2017", "end":"09.01.2017"},
        {"beg":"12.01.2017", "end":"31.01.2017"},
        {"beg":"14.06.2017", "end":"15.06.2017"},
    ]
    trip_dates = parse_trips(dates)
    process_trips(trip_dates)


def main(json_file):
    with open(json_file, 'r') as file:
        trips_json = json.load(file)
    trip_dates = parse_trips(trips_json)
    process_trips(trip_dates)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--test', action='store_true', help='Run the test function')
    parser.add_argument('--file', type=str, help='Specify the file path')
    args = parser.parse_args()
    if parser.parse_args().test:
        test()
    elif parser.parse_args().file:
        main(json_file=parser.parse_args().file)