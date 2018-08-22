#!/usr/bin/env python3

import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2
import math

from sys import argv

"""Module draw road lines on image"""

def region_of_interest(img, vertices):
    mask = np.zeros_like(img)
    match_mask_color = 255
    cv2.fillPoly(mask, vertices, match_mask_color)
    masked_image = cv2.bitwise_and(img, mask)

    return masked_image

def draw_lines(img, lines, thickness=3):
    line_img = np.zeros((img.shape[0], img.shape[1], 3), dtype=np.uint8)
    zone_img = np.zeros((img.shape[0], img.shape[1], 3), dtype=np.uint8)
    img = np.copy(img)

    if lines is None:
        return
    
    ctr = []

    for line in lines:
        for x1, y1, x2, y2 in line:
            cv2.line(line_img, (x1, y1), (x2, y2), (0, 255, 255), thickness)
            ctr += [[x1, y1], [x2, y2]]
    
    ctr[2], ctr[3] = ctr[3], ctr[2]
    cv2.fillConvexPoly(zone_img, np.array([ctr], 'int32'), (0, 255, 0))

    img = cv2.addWeighted(zone_img, 0.3, img, 1.0, 0.0)
    img = cv2.addWeighted(line_img, 0.8, img, 1.0, 0.0)
    
    return img
    
def pipeline(image):
    height = image.shape[0]
    width = image.shape[1]

    region_of_interest_vertices = [(0, height), (width / 2, height / 2), (width, height),]
    gray_image = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    cannyed_image = cv2.Canny(gray_image, 100, 200)
 
    cropped_image = region_of_interest(cannyed_image, np.array([region_of_interest_vertices], np.int32))
 
    lines = cv2.HoughLinesP(cropped_image, rho=6, theta=np.pi / 60, threshold=160, lines=np.array([]), minLineLength=40, maxLineGap=25)
 
    left_line_x = []
    left_line_y = []
    right_line_x = []
    right_line_y = []
 
    for line in lines:
        for x1, y1, x2, y2 in line:
            slope = (y2 - y1) / (x2 - x1)
            if math.fabs(slope) < 0.5:
                continue
            if slope <= 0:
                left_line_x.extend([x1, x2])
                left_line_y.extend([y1, y2])
            else:
                right_line_x.extend([x1, x2])
                right_line_y.extend([y1, y2])
    
    min_y = int(image.shape[0] * (3 / 5))
    max_y = int(image.shape[0])

    poly_left = np.poly1d(np.polyfit(left_line_y, left_line_x, deg=1))
 
    left_x_start = int(poly_left(max_y))
    left_x_end = int(poly_left(min_y))
 
    poly_right = np.poly1d(np.polyfit(right_line_y, right_line_x, deg=1))
 
    right_x_start = int(poly_right(max_y))
    right_x_end = int(poly_right(min_y))

    line_image = draw_lines(image, [[[left_x_start, max_y, left_x_end, min_y], [right_x_start, max_y, right_x_end, min_y]]], thickness=5)

    return line_image

def main():
    if len(argv) == 2:
        try:
            image = cv2.imread(argv[1], 1)
            cv2.imshow('image', pipeline(image))
            cv2.waitKey(0)
            cv2.destroyAllWindows()            
        except Exception as e:
            print('line_detect: %s' % str(e))
            exit(1)

if __name__ == '__main__':
    main()

# from moviepy.editor import VideoFileClip
# from IPython.display import HTML

# white_output = 'solidWhiteRight_output.mp4'
# clip1 = VideoFileClip("solidWhiteRight_input.mp4")
# white_clip = clip1.fl_image(pipeline)
# white_clip.write_videofile(white_output, audio=False)
