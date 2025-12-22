pipeline {
    agent any
    
    environment {
        CONAN_USER_HOME = "${WORKSPACE}/.conan"
    }
    
    stages {
        stage('Checkout') {
            steps {
                checkout scm
            }
        }
        
        stage('Install Dependencies') {
            steps {
                sh '''
                    pip3 install conan
                    conan --version
                '''
            }
        }
        
        stage('Build') {
            parallel {
                stage('Linux Build') {
                    steps {
                        sh '''
                            ./build.sh
                        '''
                    }
                }
                stage('Docker Build') {
                    steps {
                        sh '''
                            docker build -t conch-cpp:${BUILD_NUMBER} .
                            docker tag conch-cpp:${BUILD_NUMBER} conch-cpp:latest
                        '''
                    }
                }
            }
        }
        
        stage('Test') {
            steps {
                sh '''
                    ./scripts/run_tests.sh
                '''
            }
            post {
                always {
                    junit 'build/test-results/*.xml'
                }
            }
        }
        
        stage('Code Coverage') {
            steps {
                sh '''
                    ./scripts/run_tests.sh --coverage
                '''
            }
            post {
                always {
                    publishHTML([
                        reportDir: 'build/coverage',
                        reportFiles: 'index.html',
                        reportName: 'Coverage Report'
                    ])
                }
            }
        }
        
        stage('Generate Documentation') {
            steps {
                sh '''
                    ./scripts/generate_docs.sh
                '''
            }
            post {
                always {
                    publishHTML([
                        reportDir: 'docs/html',
                        reportFiles: 'index.html',
                        reportName: 'API Documentation'
                    ])
                }
            }
        }
        
        stage('Deploy to Staging') {
            when {
                branch 'develop'
            }
            steps {
                sh '''
                    docker-compose -f docker-compose.yml up -d
                '''
            }
        }
        
        stage('Deploy to Production') {
            when {
                branch 'main'
            }
            steps {
                input message: 'Deploy to production?', ok: 'Deploy'
                sh '''
                    docker-compose -f docker-compose.prod.yml up -d
                '''
            }
        }
    }
    
    post {
        success {
            echo 'Build successful!'
        }
        failure {
            echo 'Build failed!'
        }
        always {
            cleanWs()
        }
    }
}
