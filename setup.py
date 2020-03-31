def main():
    from skbuild import setup

    setup(
        name='qpoases',
        version='0.0.0',
        description='Unofficial Python bindings for qpOASES using pybind11',
        long_description=open('README.md').read(),
        long_description_content_type='text/markdown',
        url='',
        author='Lars Tingelstad',
        author_email='Lars Tingelstad@ntnu.no',
        license='LGPL-3.0',
        packages=['qpoases', '_qpoases'],
        package_dir={'': 'modules'},
        install_requires=['numpy'],
        cmake_args=[
             '-DCMAKE_BUILD_TYPE=Release',
        ],
        classifiers=[
            'OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)'
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: Implementation :: CPython',
            'Topic :: Scientific/Engineering',
        ],
    )


if __name__ == '__main__':
    main()
