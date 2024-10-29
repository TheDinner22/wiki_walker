import requests
from requests.sessions import Request

session = requests.Session()

url = "https://en.wikipedia.org/w/api.php?action=query&format=json&titles=Albert Einstein&prop=links&pllimit=max"
params = {
    "action": "query",
    "format": "json",
    "titles": "Albert Einstein",
    "prop": "links",
    "pllimit": "max"
}

response = session.get(url=url)
data = response.json()
pages = data["query"]["pages"]

pg_count = 1
page_titles = []

#print("Page %d" % pg_count)
for key, val in pages.items():
    for link in val["links"]:
        print(link["title"])
        page_titles.append(link["title"])

while "continue" in data:
    plcontinue = data["continue"]["plcontinue"]
    params["plcontinue"] = plcontinue
    #print(plcontinue)

    response = session.get(url=url, params=params)
    r = Request(url=url, params=params)
    p = r.prepare()
    #print(p.url)
    data = response.json()
    pages = data["query"]["pages"]

    pg_count += 1

    #print("\nPage %d" % pg_count)
    for key, val in pages.items():
        for link in val["links"]:
            print(link["title"])
            page_titles.append(link["title"])

print(len(page_titles))
