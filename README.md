# Tetris SDL2, with AI
A minimalist remake of the classic game Tetris using SDL2.

------------------------------------------

## [Video Demo](https://youtu.be/Gxpx3hvdxY0)

## Synopsis
I absolutely love the game Tetris and it's challenging nature. There are many remakes out there today but most lack certain features and none is perfect in my book. The aim of this project is to create a fast-paced, responsive, challenging Tetris game and pushes the player while providing useful tools to do so. 

## New Feature - Tetris AI
[AI Video Demo](https://youtu.be/uMKa24My7fE)

The AI uses a beam search. The search depth and beam width can be adjusted in [TetrisAI.h].
The AI runs in separated threads and controls the same via simulated keyboard input. 

## Features 
+ Progressive, non-linear gravity.
+ Progressive, non-linear lock delay.
+ Super Rotation System, enabling moves such as T-Spin. 
+ Ghost piece.
+ Block holding.
+ Block preview. 
+ Hard drop and soft drop. 
+ Wall-kick and floor kick. 

## Requirements
This program requires the following libraries. 
SDL2
SDL2_TTF
pthread
boost::thread

## Installation
Makefile will soon be added.


## Instructions
+ [S] to start a new game, [Q] to quit at any time. 
+ [LEFT], [DOWN] and [RIGHT] arrow keys to move the current tetromino.
+ [UP] to soft drop, [SPACE] to hard drop.
+ [Z] to rotatio anti-clockwise, [X] to rotate clockwise. 
+ [C] to hold/swap. 
+ [A] to turn AI ON/OFF

## Reference & Related Resource
1. [General Guideline](http://tetris.wikia.com/wiki/Tetris_Guideline)
2. [Super Rotation System](http://tetris.wikia.com/wiki/SRS)
3. [Simple Direct Layer 2](https://www.libsdl.org/download-2.0.php)

## License
This program is available under the [Zlib License](http://www.gzip.org/zlib/zlib_license.html).

##Disclaimer
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
