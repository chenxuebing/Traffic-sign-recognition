import os, sys, json, logging

from urllib import request
from urllib.request import urlopen
from bs4 import BeautifulSoup as bs

logging.basicConfig(level=logging.INFO)

if len(sys.argv) < 2:
	logging.error('no search query')
	sys.exit(0)


class Context:
	def __init__(self):
		self.query = sys.argv[1].replace(' ', '+')
		self.download_folder = 'images'
		self.download_path = os.path.join(self.download_folder, self.query.replace('+', '_'))
		self.headers = {
			'User-Agent': "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36"
		}
		self.download_links = []
		self.stats = {
			'links': 0,
			'downloaded': 0
		}


context = Context()
logging.info('query: {}'.format(context.query))
search_url = 'https://www.google.com.ua/search?q={}&num=20&source=lnms&tbm=isch'.format(context.query)


def create_dirs():
	if not os.path.exists(context.download_folder):
		os.mkdir(context.download_folder)
	if not os.path.exists(context.download_path):
		os.mkdir(context.download_path)


def _req(url):
	req = request.Request(url, None, headers=context.headers)
	return urlopen(req, timeout=10)


def get_links(links):
	for link in links:
		img = {
			'link': json.loads(link.text)['ou'],
			'type': json.loads(link.text)['ity'] or 'jpg'
		}
		context.stats['links'] += 1
		context.download_links.append(img)


def download_images():
	for img in context.download_links:
		try:
			logging.info('downloading img type: {} from {}'.format(img["type"], img["link"]))
			raw_img = _req(img['link']).read()

			image_path = os.path.join(context.download_path, 'img_{}.{}'.format(context.stats["downloaded"], img["type"]))
			with open(image_path, 'wb') as f:
				f.write(raw_img)

			context.stats['downloaded'] += 1
			processed = 'images processed: {}/{}'.format(context.stats["downloaded"], context.stats["links"])
			logging.info('image downloaded to {}. {}'.format(image_path, processed))

		except Exception as e:
			logging.error(e)


logging.info('searching for {}'.format(context.query))
response = _req(search_url)

logging.info('parsing response')
soup = bs(response.read(), 'html.parser')

logging.info('searching image links')
links = soup.find_all('div', {'class': 'rg_meta'})
get_links(links)
logging.info('total image links founded {}'.format(context.stats["links"]))

if not len(context.download_links):
	sys.exit(0)

logging.info('creating folders')
create_dirs()

logging.info('downloading images')
download_images()

logging.info('Done')
