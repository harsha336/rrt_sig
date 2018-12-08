# SIG Skeleton Definition

KnSkeleton

name posture_0

geofile "dudetube.m"

skeleton
root Hips
{ offset 0 0 0
  channel XPos 0 lim -500 500
  channel YPos 0 lim -500 500
  channel ZPos 0 lim -500 500
  channel Quat

  joint Spine
  { offset 0 7.43174 -1.9313
    visgeo "posture_0_spine_vis.m"
    channel Quat

    joint Spine1
    { offset -2e-005 12.1899 2.56314
      visgeo "posture_0_spine1_vis.m"
      channel Quat

      joint Spine2
      { offset 0 11.8846 -0.26729
        visgeo "posture_0_spine2_vis.m"
        channel Quat

        joint Spine3
        { offset 2e-005 12.2339 -1.919
          visgeo "posture_0_spine3_vis.m"
          channel Quat

          joint Neck
          { offset 0 10.4098 1.92957
            visgeo "posture_0_neck_vis.m"
            channel Quat

            joint Head
            { offset 7e-005 11.0733 0.3706
              visgeo "posture_0_head_vis.m"
              channel Quat

              joint Head_End
              { offset -5e-005 13.3213 -0.00812
                channel Quat
              }

              joint LeftEye
              { offset 3.01057 3.68922 6.34306
                channel Quat
              }

              joint RightEye
              { offset -3.07659 3.68929 6.34265
                channel Quat
              }
            }
          }

          joint LeftShoulder
          { offset 7 5.93676 2.0674
            visgeo "posture_0_leftshoulder_vis.m"
            channel Quat

            joint LeftArm
            { offset 10.7072 0 -1.10732
              visgeo "posture_0_leftarm_vis.m"
              channel Swing lim 110 150
              channel Twist 0 lim -90 120
              prerot axis 0 1 0 ang 90
              postrot axis 0 -1 0 ang 90

              joint LeftForeArm
              { offset 26.8053 0 0
                visgeo "posture_0_leftforearm_vis.m"
                euler ZY
                channel YRot 0 lim -160 0
                channel ZRot 0 lim -45 120
                prerot axis 0 1 0 ang 90
                postrot axis 0 -1 0 ang 90

                joint LeftHand
                { offset 23.1506 0 0
                  visgeo "posture_0_lefthand_vis.m"
                  channel Swing lim 50 90
                  prerot axis 0 1 0 ang 90
                  postrot axis 0 -1 0 ang 90

                  joint leftHandPointer
                  { offset 1.33757 0 0
                    visgeo "posture_0_lefthandpointer_vis.m"
                    channel Quat
                  }

                  joint LeftIndex1
                  { offset 8.87518 0.37901 1.77547
                    visgeo "posture_0_leftindex1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint LeftIndex2
                    { offset 3.48836 0 0
                      visgeo "posture_0_leftindex2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint LeftIndex3
                      { offset 2.29971 0 0
                        visgeo "posture_0_leftindex3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint LeftIndex4
                        { offset 2.40047 0 0
                          channel Quat
                        }
                      }
                    }
                  }

                  joint LeftMiddle1
                  { offset 8.96265 0.48215 -0.05992
                    visgeo "posture_0_leftmiddle1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint LeftMiddle2
                    { offset 4.16844 -0.00024 -0.12599
                      visgeo "posture_0_leftmiddle2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint LeftMiddle3
                      { offset 2.83092 -0.00024 -0.08641
                        visgeo "posture_0_leftmiddle3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint LeftMiddle4
                        { offset 2.6988 -0.00022 -0.08236
                          channel Quat
                        }
                      }
                    }
                  }

                  joint LeftPinky1
                  { offset 7.55349 0.48797 -3.33475
                    visgeo "posture_0_leftpinky1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint LeftPinky2
                    { offset 2.95377 -0.00033 -0.17773
                      visgeo "posture_0_leftpinky2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint LeftPinky3
                      { offset 1.64069 -0.00019 -0.09628
                        visgeo "posture_0_leftpinky3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint LeftPinky4
                        { offset 2.19252 -0.00026 -0.12294
                          channel Quat
                        }
                      }
                    }
                  }

                  joint LeftRing1
                  { offset 8.26197 0.3656 -1.81261
                    visgeo "posture_0_leftring1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint LeftRing2
                    { offset 4.14475 -0.00027 -0.06003
                      visgeo "posture_0_leftring2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint LeftRing3
                      { offset 2.25621 -0.00018 -0.02288
                        visgeo "posture_0_leftring3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint LeftRing4
                        { offset 2.34286 -0.00019 -0.0264
                          channel Quat
                        }
                      }
                    }
                  }

                  joint LeftThumb1
                  { offset 3.98621 -0.76986 4.13297
                    visgeo "posture_0_leftthumb1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint LeftThumb2
                    { offset -0.00943 0.02687 2.02569
                      visgeo "posture_0_leftthumb2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint LeftThumb3
                      { offset 0.08396 0.0372 2.25283
                        visgeo "posture_0_leftthumb3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint LeftThumb4
                        { offset 0.03723 -0.02861 1.98426
                          channel Quat
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          joint RightShoulder
          { offset -7 5.93676 2.0673
            visgeo "posture_0_rightshoulder_vis.m"
            channel Quat

            joint RightArm
            { offset -10.7073 0 -0.56284
              visgeo "posture_0_rightarm_vis.m"
              channel Swing lim 110 150
              channel Twist 0 lim -90 120
              prerot axis 0 -1 0 ang 90
              postrot axis 0 1 0 ang 90

              joint RightForeArm
              { offset -27.6421 -0.00113 0
                visgeo "posture_0_rightforearm_vis.m"
                euler ZY
                channel YRot 0 lim 0 160
                channel ZRot 0 lim -120 45
                prerot axis 0 -1 0 ang 90
                postrot axis 0 1 0 ang 90

                joint RightHand
                { offset -23.8318 -0.00098 0
                  visgeo "posture_0_righthand_vis.m"
                  channel Swing lim 50 90
                  prerot axis 0 -1 0 ang 90
                  postrot axis 0 1 0 ang 90

                  joint RightIndex1
                  { offset -8.9246 0.37971 1.68648
                    visgeo "posture_0_rightindex1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint RightIndex2
                    { offset -3.49335 0 0
                      visgeo "posture_0_rightindex2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint RightIndex3
                      { offset -2.30259 0 0
                        visgeo "posture_0_rightindex3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint RightIndex4
                        { offset -2.40484 0.05828 -0.01594
                          channel Quat
                        }
                      }
                    }
                  }

                  joint RightMiddle1
                  { offset -9.00754 0.48286 -0.14913
                    visgeo "posture_0_rightmiddle1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint RightMiddle2
                    { offset -4.17035 3e-005 0.00051
                      visgeo "posture_0_rightmiddle2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint RightMiddle3
                      { offset -2.83121 -5e-005 -0.00051
                        visgeo "posture_0_rightmiddle3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint RightMiddle4
                        { offset -2.69907 -5e-005 -0.00046
                          channel Quat
                        }
                      }
                    }
                  }

                  joint RightPinky1
                  { offset -7.5903 0.48857 -3.42046
                    visgeo "posture_0_rightpinky1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint RightPinky2
                    { offset -2.95912 0 8e-005
                      visgeo "posture_0_rightpinky2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint RightPinky3
                      { offset -1.64292 0 0
                        visgeo "posture_0_rightpinky3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint RightPinky4
                        { offset -2.19515 -2e-005 0.00903
                          channel Quat
                        }
                      }
                    }
                  }

                  joint RightRing1
                  { offset -8.30254 0.36626 -1.90007
                    visgeo "posture_0_rightring1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint RightRing2
                    { offset -4.14509 0 0
                      visgeo "posture_0_rightring2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint RightRing3
                      { offset -2.2555 0 0
                        visgeo "posture_0_rightring3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint RightRing4
                        { offset -2.34215 -3e-005 -0.00774
                          channel Quat
                        }
                      }
                    }
                  }

                  joint RightThumb1
                  { offset -4.04156 -0.76954 4.05603
                    visgeo "posture_0_rightthumb1_vis.m"
                    channel XRot 0 free
                    channel YRot 0 free
                    channel ZRot 0 free

                    joint RightThumb2
                    { offset 0 0 2.02589
                      visgeo "posture_0_rightthumb2_vis.m"
                      channel XRot 0 free
                      channel YRot 0 free
                      channel ZRot 0 free

                      joint RightThumb3
                      { offset 0 0 2.2547
                        visgeo "posture_0_rightthumb3_vis.m"
                        channel XRot 0 free
                        channel YRot 0 free
                        channel ZRot 0 free

                        joint RightThumb4
                        { offset 0 0 1.98481
                          channel Quat
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  joint LeftUpLeg
  { offset 8.91 -6.27 -2.145
    visgeo "posture_0_leftupleg_vis.m"
    channel Swing lim 65 120
    channel Twist 0 lim -100 30
    prerot axis 0.57735 0.57735 -0.57735 ang 120
    postrot axis -0.57735 -0.57735 0.57735 ang 120

    joint LeftLeg
    { offset 0 -44.8786 0
      visgeo "posture_0_leftleg_vis.m"
      euler ZY
      channel YRot 0 lim 0 160
      channel ZRot 0 lim -55 55
      prerot axis 0.57735 0.57735 -0.57735 ang 120
      postrot axis -0.57735 -0.57735 0.57735 ang 120

      joint LeftFoot
      { offset 0 -40.701 0
        visgeo "posture_0_leftfoot_vis.m"
        channel Swing lim 35 60
        prerot axis 0.57735 0.57735 -0.57735 ang 120
        postrot axis -0.57735 -0.57735 0.57735 ang 120

        joint LeftToeBase
        { offset 0 -7.70852 13.0052
          visgeo "posture_0_lefttoebase_vis.m"
          channel Quat

          joint LeftToe_End
          { offset 0 0 5.96686
            channel Quat
          }
        }
      }
    }
  }

  joint RightUpLeg
  { offset -8.91 -6.27 -2.145
    visgeo "posture_0_rightupleg_vis.m"
    channel Swing lim 65 120
    channel Twist 0 lim -30 100
    prerot axis 0.57735 0.57735 -0.57735 ang 120
    postrot axis -0.57735 -0.57735 0.57735 ang 120

    joint RightLeg
    { offset 0 -44.8787 0
      visgeo "posture_0_rightleg_vis.m"
      euler ZY
      channel YRot 0 lim 0 160
      channel ZRot 0 lim -55 55
      prerot axis 0.57735 0.57735 -0.57735 ang 120
      postrot axis -0.57735 -0.57735 0.57735 ang 120

      joint RightFoot
      { offset 0 -40.701 0
        visgeo "posture_0_rightfoot_vis.m"
        channel Swing lim 35 60
        prerot axis 0.57735 0.57735 -0.57735 ang 120
        postrot axis -0.57735 -0.57735 0.57735 ang 120

        joint RightToeBase
        { offset 6e-005 -7.70852 13.0052
          visgeo "posture_0_righttoebase_vis.m"
          channel Quat

          joint RightToe_End
          { offset 0 0.00094 5.96686
            channel Quat
          }
        }
      }
    }
  }
}

end
