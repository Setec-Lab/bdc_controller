BDC Prototype -- README {#mainpage}
============

### Most recent version of this repository can be found in: ###

* [Bitbucket](https://bitbucket.org/juanjorojash/bdc_prototype/src/master)

### Documentation ###

* Full documentation in HTLM version can be found in **documentation/htlm/index.htlm**

### What is this repository for? ###

* This repository was created to be used in conjuction with the integrated system known as "BDC_Prototype.V1.1". It was created to control the PIC16F1786 installed for feedback control, and serial logging.
* Hardware version: BDC_Prototype.V1.1

### How do I get set up? ###

* Connect usign serial cable. 
* **Serial terminal** Start with the following settings BR:19200 kbps, Data: 8 bit, Parity: none, Stop bits: 1, Flow control: none. 
* **LabView logger program** A program for receiving, saving and plotting the test data is included in the repository, in the directory **logger/main.vi**. The following programs are needed in order to run this program in a PC that **does not** have LabView installed:
	* [LabVIEW Run-Time Engine 2016 - (64-bit)](http://www.ni.com/download/labview-run-time-engine-2016/6067/en/) 
	* [NI-VISA Run-Time Engine 16.0](http://www.ni.com/download/ni-visa-run-time-engine-16.0/6188/en/)
The program creates the directory **c:/BDC_logger_data** to store the data.

### Contribution guidelines ###

* If you want to propose a review or need to modify the code for any reason first clone this [repository](https://bitbucket.org/juanjorojash/bdc_prototype/src/master) in your PC and create a new branch for your changes. Once your changes are complete and fully tested ask the administrator permission to push this new branch into the source.
* If you just want to do local changes instead you can download a zip version of the repository and do all changes locally in your PC. 

### Who do I talk to? ###

* [Juan J. Rojas](mailto:juan.rojas@itcr.ac.cr)
